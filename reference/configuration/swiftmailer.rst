.. index::
   single: Referencia de configuración; Swiftmailer

Configurando SwiftmailerBundle
==============================

Configuración predeterminada completa
-------------------------------------

.. configuration-block::

    .. code-block:: yaml

        swiftmailer:
            transport:            smtp
            username:             ~
            password:             ~
            host:                 localhost
            port:                 false
            encryption:           ~
            auth_mode:            ~
            spool:
                type:                 file
                path:                 %kernel.cache_dir%/swiftmailer/spool
            sender_domicilio:       ~
            antiflood:
                threshold:            99
                sleep:                0
            delivery_domicilio:     ~
            disable_delivery:     ~
            logging:              true