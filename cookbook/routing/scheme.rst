.. index::
   single: Enrutando; Requisito de esquema

Cómo forzar las rutas para utilizar siempre HTTPS
=================================================

A veces, deseas proteger algunas rutas y estar seguro de que siempre se accede a ellas a través del protocolo HTTPS. El componente ``Routing`` te permite forzar el esquema HTTP a través del requisito ``_scheme``:

.. configuration-block::

    .. code-block:: yaml

        protegida:
            pattern:  /protegida
            defaults: { _controller: AcmeDemoBundle:Principal:protegida }
            requirements:
                _scheme:  https

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="protegida" pattern="/protegida">
                <default key="_controller">AcmeDemoBundle:Principal:protegida</default>
                <requirement key="_scheme">https</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('protegida', new Route('/protegida', array(
            '_controller' => 'AcmeDemoBundle:Principal:protegida',
        ), array(
            '_scheme' => 'https',
        )));

        return $coleccion;

La configuración anterior obliga a utilizar siempre la ruta ``protegida`` HTTPS.

Cuando se genera la URL ``protegida``, y si el esquema actual es HTTP, Symfony generará automáticamente una URL absoluta con HTTPS como esquema:

.. code-block:: text

    # Si el esquema actual es HTTPS
    {{ path('protegida') }}
    # genera /protegida

    # Si el esquema actual es HTTP
    {{ path('protegida') }}
    # genera https://ejemplo.com/protegida

El requisito también aplica para las peticiones entrantes. Si intentas acceder a la ruta ``/protegida`` con HTTP, automáticamente se te redirige a la misma URL, pero con el esquema HTTPS.

El ejemplo anterior utiliza ``https`` para el ``_scheme``, pero también puedes obligar a que una URL siempre utilice ``http``.

.. note::

    El componente ``Security`` proporciona otra manera de hacer cumplir el esquema HTTP a través de la creación de ``requires_channel``. Este método alternativo es más adecuado para proteger "una amplia área" de tu sitio web (todas las direcciones URL en ``/admin``) o cuando deseas proteger direcciones URL definidas en un paquete de terceros.
