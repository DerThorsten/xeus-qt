#include "xq/xserver_qt.hpp"
#include <iostream>
namespace xeus
{
    ///////////////////////////////////////////////
    // xtrivial_qt_messenger implementation
    ///////////////////////////////////////////////
    xtrivial_qt_messenger::xtrivial_qt_messenger(xserver_qt* server)
    : p_server(server)
    {
    }

    xtrivial_qt_messenger::~xtrivial_qt_messenger()
    {
    }

    nl::json xtrivial_qt_messenger::send_to_shell_impl(const nl::json& message)
    {
        return p_server->my_notify_internal_listener(message);
    }


    ///////////////////////////////////////////////
    // xserver_qt implementation
    ///////////////////////////////////////////////
    xserver_qt::xserver_qt(const xconfiguration& /*config*/)
    :    
        p_messenger(new xtrivial_qt_messenger(this))
    {
    }

    xserver_qt::~xserver_qt()
    {

    }

    void xserver_qt::my_notify_shell_listener(xmessage msg)
    {
        this->notify_shell_listener(std::move(msg));
    }
    void xserver_qt::my_notify_control_listener(xmessage msg)
    {
        this->notify_control_listener(std::move(msg));
    }
    void xserver_qt::my_notify_stdin_listener(xmessage msg)
    {
        this->notify_stdin_listener(std::move(msg));
    }
    nl::json xserver_qt::my_notify_internal_listener(nl::json msg)
    {
        return this->notify_internal_listener(std::move(msg));
    }

    xcontrol_messenger& xserver_qt::get_control_messenger_impl() 
    {
        return *p_messenger;
    }

    // send from xeus to qt
    void xserver_qt::send_shell_impl(xmessage message) 
    {
        auto shared_message = std::make_shared<xmessage>(std::move(message));
        emit sig_send_shell(shared_message);
    }
    // send from xeus to qt
    void xserver_qt::send_control_impl(xmessage message) 
    {
        auto shared_message = std::make_shared<xmessage>(std::move(message));
        emit sig_send_controll(shared_message);
    }
    // send from xeus to qt
    void xserver_qt::send_stdin_impl(xmessage message) 
    {
        auto shared_message = std::make_shared<xmessage>(std::move(message));
        emit sig_send_stdin(shared_message);
    }
    // send from xeus to qt
    void xserver_qt::publish_impl(xpub_message message, channel c) 
    {
        auto shared_message = std::make_shared<xpub_message>(std::move(message));
        emit sig_publish(shared_message, c);
    }

    void xserver_qt::start_impl(xpub_message  /*message*/) 
    {
    }

    void xserver_qt::abort_queue_impl(const listener& /*l*/, long /*polling_interval*/) 
    {

    }

    void xserver_qt::stop_impl() 
    {
    }

    void xserver_qt::update_config_impl(xconfiguration& /*config*/) const 
    {
    }

    std::unique_ptr<xserver> make_xserver_qt(xcontext& /*context*/, const xconfiguration& config, nl::json::error_handler_t /*eh*/)
    {
        return std::make_unique<xserver_qt>(config);
    }

}