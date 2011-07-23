.. index::
   single: Referencia de configuración; WebProfiler

Configurando WebProfiler
========================

Configuración predeterminada completa
-------------------------------------

.. configuration-block::

    .. code-block:: yaml

        web_profiler:

            # muestra información secundaria para abreviar la barra de herramientas
            verbose:             true

            # Muestra la barra de depuración web en la parte inferior de la páginas con un resumen
# de información del generador de perfiles
            toolbar:             false

            # te da la oportunidad de analizar los datos recogidos antes de la siguiente redirección
            intercept_redirects: false