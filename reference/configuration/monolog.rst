.. index::
   pair: Monolog; Referencia de configuración

Referencia de configuración
===========================

.. configuration-block::

    .. code-block:: yaml

        monolog:
            handlers:

                # Ejemplos:
                syslog:
                    type:                stream
                    path:                /var/log/symfony.log
                    level:               ERROR
                    bubble:              false
                    formatter:           my_formatter
                main:
                    type:                fingerscrossed
                    action_level:        WARNING
                    buffer_size:         30
                    handler:             custom
                custom:
                    type:                service
                    id:                  my_handler

                # Prototipo
                name:
                    type:                 ~ # Required
                    id:                   ~
                    priority:             0
                    level:                DEBUG
                    bubble:               true
                    path:                 %kernel.logs_dir%/%kernel.environment%.log
                    ident:                false
                    facility:             user
                    max_files:            0
                    action_level:         WARNING
                    stop_buffering:       true
                    buffer_size:          0
                    handler:              ~
                    members:              []
                    from_correo:           ~
                    to_correo:             ~
                    subject:              ~
                    correo_prototype:
                        id:     ~ # Requerido (cuando se utiliza el correo_prototype)
                        method: ~
                    formatter:            ~

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="syslog"
                    type="stream"
                    path="/var/log/symfony.log"
                    level="error"
                    bubble="false"
                    formatter="my_formatter"
                />
                <monolog:handler
                    name="main"
                    type="fingerscrossed"
                    action-level="warning"
                    handler="custom"
                />
                <monolog:handler
                    name="custom"
                    type="service"
                    id="my_handler"
                />
            </monolog:config>
        </container>

.. note::

    Cuando está habilitado el generador de perfiles, se agrega un controlador para almacenar los mensajes del registro en el generador de perfiles. El generador de perfiles utiliza el nombre "debug" por lo tanto está reservado y no se puede utilizar en la configuración.
