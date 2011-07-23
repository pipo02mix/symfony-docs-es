Cómo personalizar páginas de error
==================================

Cuando se lanza alguna excepción en Symfony2, la excepción es capturada dentro de la clase ``Kernel`` y, finalmente, remitida a un controlador especial, ``TwigBundle:Exception:show`` para procesarla. Este controlador, el cual vive dentro del núcleo de ``TwigBundle``, determina cual plantilla de error mostrar y el código de estado que se debe establecer para la excepción dada.

.. tip::

    Personalizar el tratamiento de las excepciones en realidad es mucho más poderoso que lo escrito aquí. Produce un evento interno, ``core.exception``, el cual te permite completo control sobre el manejo de la excepción. Para más información, consulta el :ref:`kernel-kernel.exception`.

Todas las plantillas de error viven dentro de ``TwigBundle``. Para sustituir las plantillas, simplemente confiamos en el método estándar para sustituir las plantillas que viven dentro de un paquete. Para más información, consulta :ref:`overriding-bundle-templates`.

Por ejemplo, para sustituir la plantilla de error predeterminada mostrada al usuario final, crea una nueva plantilla ubicada en ``app/Resources/TwigBundle/views/Exception/error.html.twig``:

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Ha ocurrido un error: {{ texto_estado }}</title>
    </head>
    <body>
        <h1>¡Oops! Ha ocurrido un error</h1>
        <h2>El servidor devolvió un "{{ codigo_estado }} {{ texto_estado }}".</h2>
    </body>
    </html>

.. tip::

    Si no estás familiarizado con Twig, no te preocupes. Twig es un sencillo, potente y opcional motor de plantillas que se integra con ``Symfony2``.

Además de la página de error HTML estándar, Symfony proporciona una página de error predeterminada para muchos de los más comunes formatos de respuesta, como JSON (``error.json.twig``), XML (``error.xml.twig``) e incluso Javascript (``error.js.twig``), por nombrar algunos. Para sustituir cualquiera de estas plantillas, basta con crear un nuevo archivo con el mismo nombre en el directorio ``app/Resources/TwigBundle/views/Exception``. Esta es la manera estándar de sustituir cualquier plantilla que viva dentro de un paquete.

.. _cookbook-error-pages-by-status-code:

Personalizando la página 404 y otras páginas de error
-----------------------------------------------------

También puedes personalizar plantillas de error específicas de acuerdo con el código de estado HTTP. Por ejemplo, crea una plantilla ``app/Resources/TwigBundle/views/Exception/error404.html.twig`` para mostrar una página especial para los errores 404 (página no encontrada).

Symfony utiliza el siguiente algoritmo para determinar qué plantilla utilizar:

* En primer lugar, busca una plantilla para el formato dado y el código de estado (como ``error404.json.twig``);

* Si no existe, busca una plantilla para el formato propuesto (como ``error.json.twig``);

* Si no existe, este cae de nuevo a la plantilla HTML (como ``error.html.twig``).

.. tip::

    Para ver la lista completa de plantillas de error predeterminadas, revisa el directorio ``Resources/views/Exception`` de ``TwigBundle``. En una instalación estándar de Symfony2, el ``TwigBundle`` se puede encontrar en ``vendor/symfony/src/Symfony/Bundle/TwigBundle``. A menudo, la forma más fácil de personalizar una página de error es copiarla de ``TwigBundle`` a ``app/Resources/TwigBundle/views/Exception`` y luego modificarla.

.. note::

    El amigable depurador de páginas de excepción muestra al desarrollador cómo, incluso, puede personalizar de la misma manera creando plantillas como ``exception.html.twig`` para la página de excepción HTML estándar o ``exception.json.twig`` para la página de excepción JSON.
