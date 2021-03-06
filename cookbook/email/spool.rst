Cómo organizar el envío de correo electrónico
=============================================

Cuando estás utilizando el ``SwiftmailerBundle`` para enviar correo electrónico desde una aplicación Symfony2, de manera predeterminada el mensaje será enviado inmediatamente. Sin embargo, posiblemente quieras evitar el impacto en el rendimiento de la comunicación entre ``SwiftMailer`` y el transporte de correo electrónico, lo cual podría hacer que el usuario tuviera que esperar la carga de la siguiente página, mientras que se envía el correo electrónico. Puedes evitar todo esto eligiendo "carrete", los correos electrónicos en lugar de enviarlos directamente. Esto significa que ``SwiftMailer`` no intenta enviar el correo electrónico, sino que guarda el mensaje en alguna parte, tal como un archivo. Otro proceso puede leer de la cola y hacerse cargo de enviar los correos que están organizados en la cola. Actualmente, sólo cola de archivo es compatible con ``SwiftMailer``.

Para utilizar la cola, utiliza la siguiente configuración:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            # ...
            spool:
                type: file
                path: /ruta/a/la/cola

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config>
             <swiftmailer:spool
                 type="file"
                 path="/ruta/a/la/cola" />
        </swiftmailer:config>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('swiftmailer', array(
             // ...
            'spool' => array(
                'type' => 'file',
                'path' => '/ruta/a/la/cola',
            )
        ));

.. tip::

    Si deseas almacenar la cola de correo en algún lugar en el directorio de tu proyecto, recuerda que puedes utilizar el parámetro `%kernel.root_dir%` para referirte a la raíz del proyecto:

    .. code-block:: yaml

        path: %kernel.root_dir%/app/spool

Ahora, cuando tu aplicación envía un correo electrónico, no se enviará realmente, sino que se añade a la cola de correo. El envío de los mensajes desde la cola se hace por separado.
Hay una orden de consola para enviar los mensajes en la cola de correo:

.. code-block:: bash

    php app/console swiftmailer:spool:send

Tiene una opción para limitar el número de mensajes que se enviarán:

.. code-block:: bash

    php app/console swiftmailer:spool:send --message-limit=10

También puedes establecer el límite de tiempo en segundos:

.. code-block:: bash

    php app/console swiftmailer:spool:send --time-limit=10

Por supuesto que en realidad no deseas ejecutar esto manualmente. En cambio, la orden de consola se debe activar por un trabajo cronometrado o tarea programada y ejecutarse a intervalos regulares.
