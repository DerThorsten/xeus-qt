#include "xq/xqServer.hpp"

// STL includes
#include <memory>
#include <thread>
#include <iostream>

// zmq includes
#include <zmq_addon.hpp>

// xeus includes
#include <xeus/xserver_zmq.hpp>

// Qt includes
#include <QDebug>
#include <QTimer>

xqServer::xqServer(zmq::context_t& context,
                   const xeus::xconfiguration& c,
                   nl::json::error_handler_t eh)
    : xserver_zmq(context, c, eh)
{
    // 10ms interval is short enough so that users will not notice significant latency
    // yet it is long enough to minimize CPU load caused by polling.
    // 50ms causes too long delay in interactive widgets that handle mousemove events.
    m_pollTimer = new QTimer();
    m_pollTimer->setInterval(10);
    QObject::connect(m_pollTimer, &QTimer::timeout, [=]() { poll(0); });
}

xqServer::~xqServer()
{
    m_pollTimer->stop();
    delete m_pollTimer;
}

void xqServer::start_impl(xeus::xpub_message message)
{
    start_publisher_thread();
    start_heartbeat_thread();

    m_request_stop = false;

    m_pollTimer->start();

    publish(std::move(message), xeus::channel::SHELL);
}

void xqServer::stop_impl()
{
    this->xserver_zmq::stop_impl();
    m_pollTimer->stop();
    stop_channels();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

std::unique_ptr<xeus::xserver> make_xqServer(xeus::xcontext& context,
                                             const xeus::xconfiguration& config,
                                             nl::json::error_handler_t eh)
{
    return std::make_unique<xqServer>(context.get_wrapped_context<zmq::context_t>(), config, eh);
}

void xqServer::setPollIntervalSec(double intervalSec)
{
    m_pollTimer->setInterval(intervalSec*1000.0);
}

double xqServer::pollIntervalSec()
{
    return m_pollTimer->interval() / 1000.0;
}
