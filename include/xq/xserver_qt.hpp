#ifndef XEUS_SERVER_QT_HPP
#define XEUS_SERVER_QT_HPP

#include "xeus/xeus.hpp"
#include "xeus/xserver.hpp"
#include "xeus/xeus_context.hpp"
#include "xeus/xkernel_configuration.hpp"

#include "xq/xq.hpp"

#include <QObject>


namespace nl = nlohmann;
namespace xeus
{

    class xserver_qt;

    class XQ_API xtrivial_qt_messenger : public xcontrol_messenger
    {
    public:
        
        explicit xtrivial_qt_messenger(xserver_qt* server);
        virtual ~xtrivial_qt_messenger ();

    private:

        nl::json send_to_shell_impl(const nl::json& message) override;
        xserver_qt* p_server;
    };

    class XQ_API xserver_qt :  public QObject, public xserver
    {
        // Some Qt macro magic
        Q_OBJECT

    public:
        xserver_qt()
        :   QObject(),
            xserver(){
        }

        xserver_qt(const xconfiguration& config);
        ~xserver_qt();

        
        void my_notify_shell_listener(xmessage msg) ;
        void my_notify_control_listener(xmessage msg) ;
        void my_notify_stdin_listener(xmessage msg) ;
        nl::json my_notify_internal_listener(nl::json msg) ;


    signals:

        void sig_send_shell(std::shared_ptr<xmessage>);
        void sig_send_controll(std::shared_ptr<xmessage>);
        void sig_send_stdin(std::shared_ptr<xmessage>);

        void sig_publish(std::shared_ptr<xpub_message>, channel);

        // using xserver::notify_shell_listener;
        // using xserver::notify_control_listener;
        // using xserver::notify_stdin_listener;
        // using xserver::notify_internal_listener;


    protected:

        xcontrol_messenger& get_control_messenger_impl() override;

        void send_shell_impl(xmessage message) override;
        void send_control_impl(xmessage message) override;
        void send_stdin_impl(xmessage message) override;
        void publish_impl(xpub_message message, channel c) override;

        void start_impl(xpub_message message) override;
        void abort_queue_impl(const listener& l, long polling_interval) override;
        void stop_impl() override;
        void update_config_impl(xconfiguration& config) const override;

        using trivial_messenger_ptr = std::unique_ptr<xtrivial_qt_messenger>;
        trivial_messenger_ptr p_messenger;

    };


    XEUS_API
    std::unique_ptr<xserver> make_xserver_qt(xcontext& context, const xconfiguration& config, nl::json::error_handler_t eh = nl::json::error_handler_t::strict);
}

#endif