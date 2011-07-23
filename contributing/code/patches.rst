Enviando un parche
==================

Los parches son la mejor manera de proporcionar una corrección de error o de proponer mejoras a Symfony2.

Configuración inicial
---------------------

Antes de trabajar en Symfony2, configura un entorno amigable con el siguiente software:

* Git;

* PHP versión 5.3.2 o superior;

* PHPUnit 3.5.11 o superior.

Configura la información del usuario con tu nombre real y una dirección de correo electrónico operativa:

.. code-block:: bash

    $ git config --global user.name "Tu nombre"
    $ git config --global user.correo tu@ejemplo.com

.. tip::

    Si eres nuevo en Git, es muy recomendable que leas el excelente libro `ProGit`_, que además es libre.

Obtén el código fuente de Symfony2:

* Crea una cuenta `GitHub`_ e ingresa;

* Consigue el `repositorio Symfony2`_ (haz clic en el botón "Fork");

* Después de concluida la "acción de bifurcar el núcleo", clona tu bifurcación a nivel local (esto creará un directorio `symfony`):

.. code-block:: bash

      $ git clone git@github.com:NOMBREUSUARIO/symfony.git

* Añade el repositorio anterior como ``remoto``:

.. code-block:: bash

      $ cd symfony
      $ git remote add upstream git://github.com/symfony/symfony.git

Ahora que Symfony2 está instalado, comprueba que todas las pruebas unitarias pasan en tu entorno como se explica en el :doc:`documento <tests>` dedicado.

Trabajando en un parche
-----------------------

Cada vez que desees trabajar en un parche por un fallo o una mejora, crea una rama tema:

.. code-block:: bash

    $ git checkout -b NOMBRE_RAMA

.. tip::

    Utiliza un nombre descriptivo para tu rama (`tarjeta_XXX` donde `XXX` es el número de tarjeta es una buena convención para la corrección de errores).

La orden anterior automáticamente cambia el código de la rama recién creada (para ver la sección en que estás trabajando utiliza `git branch`).

Trabaja en el código tanto como quieras y consigna tanto como desees, pero ten en cuenta lo siguiente:

* Sigue los :doc:`estándares <standards>` de codificación (utiliza `git diff --check` para comprobar si hay espacios finales);

* Añade pruebas unitarias para probar que el error se corrigió o que la nueva característica realmente funciona;

* Haz consignaciones atómicas y separadas lógicamente (usa el poder de `git rebase` para tener un historial limpio y lógico);

* Escribe mensajes de consignación sustanciales.

.. tip::

    Un buen mensaje de consignación sustancial está compuesto por un resumen (la primera línea), seguido opcionalmente por una línea en blanco y una descripción más detallada. El resumen debe comenzar con el componente en el que estás trabajando entre corchetes (``[DependencyInjection]``, ``[FrameworkBundle]``, ...). Utiliza un verbo (``fixed...``, ``added...``, ...) para iniciar el resumen y no agregues un punto al final.

Enviando un parche
------------------

Antes de presentar tu revisión, actualiza tu rama (es necesario si te toma cierto tiempo terminar los cambios):

.. code-block:: bash

    $ git checkout master
    $ git fetch upstream
    $ git merge upstream/master
    $ git checkout NOMBRE_RAMA
    $ git rebase master

Al ejecutar la orden ``rebase``, posiblemente tengas que arreglar conflictos de fusión.
``git status`` te mostrará los archivos *sin fusionar*. Resuelve todos los conflictos, y luego continua el rebase:

.. code-block:: bash

    $ git add ... # Añade archivos resueltos
    $ git rebase --continue

Comprueba que todas las pruebas todavía pasan y empuja tu rama remota:

.. code-block:: bash

    $ git push origin NOMBRE_RAMA

Ahora puedes hablar de tu parche en la `lista de correo dev`_ o hacer una petición de atracción (que se debe hacer en el repositorio ``symfony/synfony``). Para facilitar el trabajo del equipo central, siempre incluye los componentes modificados en tu mensaje de petición de atracción, como en:

.. code-block:: text

    [Yaml] foo bar
    [Form] [Validator] [FrameworkBundle] foo bar

Si vas a enviar un correo electrónico a la lista de correo, no olvides hacer referencia a la URL de tu rama (``https://github.com/NOMBREDEUSUARIO/symfony.git NOMBRE_RAMA``) o la URL de la petición de atracción.

Basándote en la retroalimentación de la lista de correo o a través de la petición de atracción en GitHub, posiblemente tengas que rehacer el parche. Antes de volver a presentar la revisión, rebasa con el maestro, sin fusionar y fuerza el empuje al origen:

.. code-block:: bash

    $ git rebase -f upstream/master
    $ git push -f origin NOMBRE_RAMA

.. note::

    Todos los parches que se van a presentar se deben liberar bajo la licencia MIT, a menos que explícitamente se especifique en el código.

.. _ProGit:              http://progit.org/book/es/
.. _GitHub:              https://github.com/signup/free
.. _`repositorio Symfony2`: https://github.com/symfony/symfony
.. _`lista de correo dev`:    http://groups.google.com/group/symfony-devs
