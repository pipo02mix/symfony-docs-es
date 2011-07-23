Colaborando en la documentación
===============================

La documentación es tan importante como el código. Esta sigue exactamente los mismos principios: una vez y sólo una, pruebas, facilidad de mantenimiento,
extensibilidad, optimización y reconstrucción sólo por nombrar algunos. Y, por supuesto, la documentación tiene errores, errores tipográficos, guías difíciles de leer y mucho más.

Colaborando
-----------

Antes de colaborar, necesitas familiarizarte con: el :doc:`lenguaje de marcado <format>` empleado en la documentación.

La documentación Symfony2 se encuentra alojada en GitHub:

.. code-block:: text

    https://github.com/symfony/symfony-docs

Si deseas enviar un parche `bifurcado`_ al repositorio oficial en GitHub y luego reproducir tu bifurcación:

.. code-block:: bash

    $ git clone git://github.com/TUNOMBRE/symfony-docs.git

A continuación, crea una rama dedicada a tus cambios (para mantener la organización):

.. code-block:: bash

    $ git checkout -b mejorando_foo_y_bar

Ahora puedes hacer los cambios directamente en esta rama y consignarlos ahí. Cuando hayas terminado, impulsa esta rama a *tu* GitHub e inicia una petición de atracción.
La petición de atracción debe ser entre tu rama ``mejorando_foo_y_bar`` y la rama ``maestra`` de ``Symfony-docs``.

.. image:: /images/docs-pull-request.png
   :align: center

GitHub aborda el tema de las `peticiones de atracción`_ en detalle.

.. note::

    La documentación de Symfony2 está bajo una licencia Creative Commons Attribution-Share Alike 3.0 Unported :doc:`Licencia <license>`.

Informando un problema
----------------------

La contribución más fácil que puedes hacer es informar algún problema: un error, un error gramatical, un error en el código de ejemplo, una explicación omitida, y así sucesivamente.

Pasos a seguir:

* Reporta un error en el rastreador de errores;

* *(Opcional)* Envía un parche.

Traduciendo
-----------

Lee el :doc:`documento dedicado <translations>`.

.. _`bifurcado`:            http://help.github.com/fork-a-repo/
.. _`peticiones de atracción`:  http://help.github.com/pull-requests/
