Cómo trabajar con correos electrónicos durante el desarrollo
============================================================

Cuando estás creando una aplicación que envía mensajes de correo electrónico, a menudo, mientras desarrollas, no quieres enviar realmente los correos electrónicos al destinatario especificado. Si estás utilizando el ``SwiftmailerBundle`` con Symfony2, puedes lograr fácilmente esto a través de ajustes de configuración sin tener que realizar ningún cambio en el código de tu aplicación en absoluto. Hay dos opciones principales cuando se trata del manejo de correos electrónicos durante el desarrollo: (a) desactivar el envío de correos electrónicos por completo o (b) enviar todos los mensajes de correo electrónico a una dirección especificada.

Desactivando el envío
---------------------

Puedes desactivar el envío de correos electrónicos estableciendo la opción ``disable_delivery`` a ``true``. Este es el predeterminado en el entorno ``test`` de la distribución estándar. Si haces esto en la configuración específica ``test``, los mensajes de correo electrónico no se enviarán cuando se ejecutan pruebas, pero se seguirán enviando en los entornos ``prod`` y ``dev``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        swiftmailer:
            disable_delivery:  true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            disable-delivery="true" />

    .. code-block:: php

        // app/config/config_test.php
        $contenedor->loadFromExtension('swiftmailer', array(
            'disable_delivery'  => "true",
        ));

Si también deseas inhabilitar el envío en el entorno ``dev``, sólo tienes que añadir esta configuración en el archivo ``config_dev.yml``.

Enviando a una dirección específica
-----------------------------------

También puedes optar por que todos los correos sean enviados a una dirección específica, en vez de la dirección real especificada cuando se envía el mensaje. Esto se puede hacer a través de la opción ``delivery_domicilio``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            delivery_domicilio:  dev@ejemplo.com

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            delivery-domicilio="dev@ejemplo.com" />

    .. code-block:: php

        // app/config/config_dev.php
        $contenedor->loadFromExtension('swiftmailer', array(
            'delivery_domicilio'  => "dev@ejemplo.com",
        ));

Ahora, supongamos que estás enviando un correo electrónico a ``recipiente@ejemplo.com``.

.. code-block:: php

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

En el entorno ``dev``, el correo electrónico será enviado a ``dev@ejemplo.com``.
``SwiftMailer`` añadirá un encabezado adicional para el correo electrónico, ``X-Swift-To`` conteniendo la dirección reemplazada, por lo tanto todavía serás capaz de ver que se habría enviado.

.. note::

    Además de las direcciones ``para``, también se detendrá el correo electrónico que se envíe a cualquier dirección ``CC`` y ``BCC`` establecida. ``SwiftMailer`` agregará encabezados adicionales al correo electrónico con las direcciones reemplazada en ellos.
    Estas son ``X-Swift-CC`` y ``X-Swift-CCO`` para las direcciones ``CC`` y ``BCC``, respectivamente.

Visualizando desde la barra de depuración web
---------------------------------------------

Puedes ver cualquier correo electrónico enviado por una página cuando estás en el entorno ``dev`` usando la barra de depuración web. El icono de correo electrónico en la barra de herramientas mostrará cuántos correos electrónicos fueron enviados. Si haces clic en él, se abrirá un informe mostrando los detalles de los mensajes de correo electrónico.

Si estás enviando un correo electrónico e inmediatamente después lo desvías, tendrás que establecer la opción ``intercept_redirects`` a ``true`` en el archivo ``config_dev.yml`` para que puedas ver el correo electrónico en la barra de depuración antes de que sea redirigido. 