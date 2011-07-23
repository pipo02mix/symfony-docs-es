.. index::
   single: Enrutado; Permite / en el parámetro ruta

Cómo permitir un carácter "/" en un parámetro de ruta
===============================================================

A veces, es necesario componer las URL con parámetros que pueden contener una barra inclinada ``/``. Por ejemplo, tomemos la ruta clásica ``/hola/{nombre}``. Por omisión, ``/hola/Fabien`` coincidirá con esta ruta pero no ``/hola/Fabien/Kris``. Esto se debe a que Symfony utiliza este carácter como separador entre las partes de la ruta.

Esta guía explica cómo puedes modificar una ruta para que ``/hola/Fabien/Kris`` coincida con la ruta ``/hola/{nombre}``, donde ``{nombre}`` es igual a ``Fabien/Kris``.

Configurando la ruta
--------------------

De manera predeterminada, el componente de enrutado de Symfony requiere que los parámetros coincidan con los siguientes patrones de expresiones regulares: ``[^/]+``. Esto significa que todos los caracteres están permitidos excepto ``/``. 

Debes permitir el carácter ``/`` explícitamente para que sea parte de tu parámetro especificando un patrón de expresión regular más permisivo.

.. configuration-block::

    .. code-block:: yaml

        _hola:
            pattern: /hola/{nombre}
            defaults: { _controller: AcmeDemoBundle:Demo:hola }
            requirements:
                nombre: ".+"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_hola" pattern="/hola/{nombre}">
                <default key="_controller">AcmeDemoBundle:Demo:hola</default>
                <requirement key="nombre">.+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $coleccion = new RouteCollection();
        $coleccion->add('_hola', new Route('/hola/{nombre}', array(
            '_controller' => 'AcmeDemoBundle:Demo:hola',
        ), array(
            'nombre' => '.+',
        )));

        return $coleccion;

    .. code-block:: php-annotations

        use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

        class DemoController
        {
            /**
             * @Route("/hola/{nombre}", name="_hola", requirements={"nombre" = ".+"})
             */
            public function holaAction($nombre)
            {
                // ...
            }
        }

¡Eso es todo! Ahora, el parámetro ``{nombre}`` puede contener el carácter ``/``.