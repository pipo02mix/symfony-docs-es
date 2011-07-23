.. index::
   single: Correo electrónico

Cómo enviar correo electrónico
==============================

El envío de correo electrónico es una tarea clásica para cualquier aplicación web, y la cual tiene complicaciones especiales y peligros potenciales. En lugar de recrear la rueda, una solución para enviar mensajes de correo electrónico es usando ``SwiftmailerBundle``, la cual aprovecha el poder de la biblioteca `SwiftMailer`_.

.. note::

    No olvides activar el paquete en tu núcleo antes de usarlo::

        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            );

            // ...
        }

.. _swift-mailer-configuration:

Configurando
------------

Antes de usar SwiftMailer, asegúrate de incluir su configuración. El único parámetro de configuración obligatorio es ``transport``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            transport:  smtp
            encryption: ssl
            auth_mode:  login
            host:       smtp.gmail.com
            username:   your_username
            password:   your_password

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="smtp"
            encryption="ssl"
            auth-mode="login"
            host="smtp.gmail.com"
            username="your_username"
            password="your_password" />

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('swiftmailer', array(
            'transport'  => "smtp",
            'encryption' => "ssl",
            'auth_mode'  => "login",
            'host'       => "smtp.gmail.com",
            'username'   => "your_username",
            'password'   => "your_password",
        ));

La mayoría de los atributos de configuración de SwiftMailer tratan con la forma en que se deben entregar los mensajes.

Los atributos de configuración disponibles son las siguientes:

* ``transport``         (``smtp``, ``mail``, ``sendmail`` o ``gmail``)
* ``username``
* ``password``
* ``host``
* ``port``
* ``encryption``        (``tls`` o ``ssl``)
* ``auth_mode``         (``plain``, ``login`` o ``cram-md5``)
* ``spool``

  * ``type`` (cómo formar los mensajes, actualmente sólo es compatible con ``file``)
  * ``path`` (dónde almacenar los mensajes)
* ``delivery_domicilio``  (una dirección de correo electrónico de donde enviar todos los mensajes)
* ``disable_delivery``  (fija a true para desactivar la entrega por completo)

Enviando correo electrónico
---------------------------

La biblioteca SwiftMailer trabaja creando, configurando y luego enviando objetos ``Swift_Message``. El "mailer" es responsable de la entrega real del mensaje y es accesible a través del servicio ``mailer``. En general, el envío de un correo electrónico es bastante sencillo::

    public function indexAction($nombre)
    {
        $mensaje = \Swift_Message::newInstance()
            ->setSubject('Hola correo-e')
            ->setFrom('send@ejemplo.com')
            ->setTo('recipient@ejemplo.com')
            ->setCuerpo($this->renderView('HolaBundle:Hola:email.txt.twig', array('nombre' => $nombre)))
        ;
        $this->get('mailer')->send($mensaje);

        return $this->render(...);
    }

Para mantener las cosas disociadas, el cuerpo del correo electrónico se ha almacenado en una plantilla y reproducido con el método ``RenderView()``.

El objeto ``$message`` admite muchas más opciones, como incluir archivos adjuntos, agregar contenido HTML, y mucho más. Afortunadamente, SwiftMailer cubre el tema con gran detalle en `Creando mensajes`_ de su documentación.

.. tip::

    Hay disponibles varios artículos en el recetario relacionados con el envío de mensajes de correo electrónico en Symfony2:

    * :doc:`gmail`
    * :doc:`email/dev_environment`

.. _`Swiftmailer`: http://www.swiftmailer.org/
.. _`Creando mensajes`: http://swiftmailer.org/docs/messages