.. index::
   single: Validación

Validando
=========

La validación es una tarea muy común en aplicaciones web. Los datos introducidos en formularios se tienen que validar. Los datos también se deben validar antes de escribirlos en una base de datos o pasarlos a un servicio web.

Symfony2 viene con un componente `Validator`_ que facilita esta tarea transparentemente.
Este componente está basado en la `especificación JSR303 Bean Validation`_. ¿Qué?
¿Una especificación de Java en PHP? Has oído bien, pero no es tan malo como suena.
Vamos a ver cómo se puede utilizar en PHP.

.. index:
   single: Validación; Fundamentos

Fundamentos de la validación
----------------------------

La mejor manera de entender la validación es verla en acción. Para empezar, supongamos que hemos creado un objeto plano en PHP el cual en algún lugar tiene que utilizar tu aplicación:

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Autor.php
    namespace Acme\BlogBundle\Entity;

    class Autor
    {
        public $nombre;
    }

Hasta ahora, esto es sólo una clase ordinaria que sirve a algún propósito dentro de tu aplicación. El objetivo de la validación es decir si o no los datos de un objeto son válidos. Para que esto funcione, debes configurar una lista de reglas (llamada :ref:`constraints -restricciones <validation-constraints>`) que el objeto debe seguir para ser válido. Estas reglas se pueden especificar a través de una serie de formatos diferentes (YAML, XML, anotaciones o PHP).

Por ejemplo, para garantizar que la propiedad ``$nombre`` no esté vacía, agrega lo siguiente:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            properties:
                nombre:
                    - NotBlank: ~

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Autor">
            <property name="nombre">
                <constraint name="NotBlank" />
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\NotBlank()
             */
            public $nombre;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Autor.php

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Autor
        {
            public $nombre;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('nombre', new NotBlank());
            }
        }

.. tip::

    Las propiedades protegidas y privadas también se pueden validar, así como los métodos "get" (consulta la sección :ref:`validator-constraint-targets`).

.. index::
   single: Validación; Usando el validador

Usando el servicio ``validador``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A continuación, para validar realmente un objeto ``Autor``, utiliza el método ``validate`` del servicio ``validador`` (clase :class:`Symfony\\Component\\Validator\\Validator`).
El trabajo del ``validador`` es fácil: lee las restricciones (es decir, las reglas) de una clase y comprueba si los datos en el objeto satisfacen esas restricciones. Si la validación falla, devuelve un arreglo de errores. Toma este sencillo ejemplo desde el interior de un controlador:

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;
    use Acme\BlogBundle\Entity\Autor;
    // ...

    public function indexAction()
    {
        $autor = new Autor();
        // ... haz algo con el objeto $autor

        $validador = $this->get('validator');
        $listaErrores = $validador->validate($autor);

        if (count($listaErrores) > 0) {
            return new Response(print_r($listaErrores, true));
        } else {
            return new Response('¡El autor es válido! ¡Sí!');
        }
    }

Si la propiedad ``$nombre`` está vacía, verás el siguiente mensaje de error:

.. code-block:: text

    Acme\BlogBundle\Autor.nombre:
        Este valor no debe estar en blanco

Si insertas un valor en la propiedad ``nombre``, aparecerá el satisfactorio mensaje de éxito.

.. tip::

    La mayor parte del tiempo, no interactúas directamente con el servicio ``validador`` o necesitas preocuparte por imprimir los errores. La mayoría de las veces, vas a utilizar la validación indirectamente al manejar los datos de formularios presentados. Para más información, consulta la sección :ref:`book-validation-forms`.

También puedes pasar la colección de errores a una plantilla.

.. code-block:: php

    if (count($listaErrores) > 0) {
        return $this->render('AcmeBlogBundle:Autor:validate.html.twig', array(
            'listaErrores' => $listaErrores,
        ));
    } else {
        // ...
    }

Dentro de la plantilla, puedes sacar la lista de errores exactamente como la necesites:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Autor/validate.html.twig #}

        <h3>El autor tiene los siguientes errores</h3>
        <ul>
        {% for error in listaErrores %}
            <li>{{ error.message }}</li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <!-- src/Acme/BlogBundle/Resources/views/Autor/validate.html.php -->

        <h3>El autor tiene los siguientes errores</h3>
        <ul>
        <?php foreach ($listaErrores as $error): ?>
            <li><?php echo $error->getMessage() ?></li>
        <?php endforeach; ?>
        </ul>

.. note::

    Cada error de validación (conocido cómo "violación de restricción"), está representado por un objeto :class:`Symfony\\Component\\Validator\\ConstraintViolation`.

.. index::
   single: Validación; Validación con formularios

.. _book-validation-forms:

Validación y formularios
~~~~~~~~~~~~~~~~~~~~~~~~

Puedes utilizar el servicio ``validator`` en cualquier momento para validar cualquier objeto.
En realidad, sin embargo, por lo general al trabajar con formularios vas a trabajar con el ``validador`` indirectamente. La biblioteca de formularios de Symfony utiliza internamente el servicio ``validador`` para validar el objeto subyacente después de que los valores se han presentado y vinculado. Las violaciones de restricción en el objeto se convierten en objetos ``FieldError`` los cuales puedes mostrar fácilmente en tu formulario. El flujo de trabajo típico en la presentación del formulario se parece a lo siguiente visto desde el interior de un controlador::

    use Acme\BlogBundle\Entity\Autor;
    use Acme\BlogBundle\Form\AutorType;
    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function updateAction(Request $peticion)
    {
        $autor = new Acme\BlogBundle\Entity\Autor();
        $formulario = $this->createForm(new AutorType(), $autor);

        if ($peticion->getMethod() == 'POST') {
            $formulario->bindRequest($peticion);

            if ($formulario->isValid()) {
                // pasó la validación, haz algo con el objeto $autor

                $this->redirect($this->generateUrl('...'));
            }
        }

        return $this->render('BlogBundle:Autor:form.html.twig', array(
            'form' => $formulario->createView(),
        ));
    }

.. note::

    Este ejemplo utiliza un formulario de la clase ``AutorType``, el cual no mostramos aquí.

Para más información, consulta el capítulo :doc:`Formularios </book/forms>`.

.. index::
   pair: Validación; Configuración

.. _book-validation-configuration:

Configurando
------------

El validador de Symfony2 está activado por omisión, pero debes habilitar explícitamente las anotaciones si estás utilizando el método de anotación para especificar tus restricciones:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            validation: { enable_annotations: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:validation enable_annotations="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $contenedor->loadFromExtension('framework', array('validation' => array(
            'enable_annotations' => true,
        ));

.. index::
   single: Validación; Restricciones

.. _validation-constraints:

Restricciones
-------------

El ``validador`` está diseñado para validar objetos contra *restricciones* (es decir, reglas). A fin de validar un objeto, basta con asignar una o más restricciones a tu clase y luego pasarla al servicio ``validador``.

Detrás del escenario, una restricción es simplemente un objeto PHP que hace una declaración afirmativa. En la vida real, una restricción podría ser: "El pastel no se debe quemar".
En Symfony2, las restricciones son similares: son aserciones de que una condición es verdadera. Dado un valor, una restricción te dirá si o no el valor se adhiere a las reglas de tu restricción.

Restricciones compatibles
~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 viene con un gran número de las más comunes restricciones necesarias.
La lista completa de restricciones con detalles está disponible en la :doc:`sección referencia de restricciones </reference/constraints>`.

.. index::
   single: Validación; Configurando restricciones

.. _book-validation-constraint-configuration:

Configurando restricciones
~~~~~~~~~~~~~~~~~~~~~~~~~~

Algunas restricciones, como :doc:`NotBlank</reference/constraints/NotBlank>`, son simples, mientras que otras, como la restricción :doc:`Choice</reference/constraints/Choice>`, tienen varias opciones de configuración disponibles. Supongamos que la clase ``Autor`` tiene otra propiedad, ``género`` que se puede configurar como "masculino" o "femenino":

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            properties:
                genero:
                    - Choice: { choices: [masculino, femenino], message: Elige un género válido. }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Autor">
            <property name="genero">
                <constraint name="Choice">
                    <option name="choices">
                        <value>masculino</value>
                        <value>femenino</value>
                    </option>
                    <option name="message">Elige un género válido.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Choice(
             *     choices = { "masculino", "femenino" },
             *     message = "Elige un género válido."
             * )
             */
            public $genero;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Autor
        {
            public $genero;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('genero', new Choice(array(
                    'choices' => array('masculino', 'femenino'),
                    'message' => 'Elige un género válido.',
                ));
            }
        }

Las opciones de una restricción siempre se pueden pasar como una matriz. Algunas restricciones, sin embargo, también te permiten pasar el valor de una opción "*predeterminada*", en lugar del arreglo. En el caso de la restricción ``Choice``, las ``opciones`` se pueden especificar de esta manera.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            properties:
                genero:
                    - Choice: [masculino, femenino]

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Autor">
            <property name="genero">
                <constraint name="Choice">
                    <value>masculino</value>
                    <value>femenino</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Choice({"masculino", "femenino"})
             */
            protected $genero;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;

        class Autor
        {
            protected $genero;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('genero', new Choice(array('masculino', 'femenino')));
            }
        }

Esto, simplemente está destinado a hacer que la configuración de las opciones más comunes de una restricción sea más breve y rápida.

Si alguna vez no está seguro de cómo especificar una opción, o bien consulta la documentación de la API por la restricción o juega a lo seguro pasando siempre las opciones en un arreglo (el primer método se muestra más arriba).

.. index::
   single: Validación; Objetivos de restricción

.. _validator-constraint-targets:

Objetivos de restricción
------------------------

Las restricciones se pueden aplicar a una propiedad de clase (por ejemplo, ``nombre``) o a un método captador público (por ejemplo ``getNombreCompleto``). El primero es el más común y fácil de usar, pero el segundo te permite especificar reglas de validación más complejas.

.. index::
   single: Validación; Restringiendo propiedades

Propiedades
~~~~~~~~~~~

La validación de propiedades de clase es la técnica de validación más básica. Symfony2 te permite validar propiedades privadas, protegidas o públicas. El siguiente listado muestra cómo configurar la propiedad ``$primerNombre`` de una clase ``Autor`` para que por lo menos tenga 3 caracteres.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            properties:
                nombreDePila:
                    - NotBlank: ~
                    - MinLength: 3

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Autor">
            <property name="nombreDePila">
                <constraint name="NotBlank" />
                <constraint name="MinLength">3</constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\NotBlank()
             * @Assert\MinLength(3)
             */
            private $nombreDePila;
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class Autor
        {
            private $nombreDePila;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('nombreDePila', new NotBlank());
                $metadatos->addPropertyConstraint('nombreDePila', new MinLength(3));
            }
        }

.. index::
   single: Validación; Restricciones a proveedores

Captadores
~~~~~~~~~~

Las restricciones también se pueden aplicar al valor devuelto por un método. Symfony2 te permite agregar una restricción a cualquier método público cuyo nombre comience con ``get`` o ``is``. En esta guía, ambos métodos de este tipo son conocidos como "captadores" o ``getters``.

La ventaja de esta técnica es que te permite validar el objeto de forma dinámica. Por ejemplo, supongamos que quieres asegurarte de que un campo de contraseña no coincide con el nombre del usuario (por razones de seguridad). Puedes hacerlo creando un método ``isPaseLegal``, a continuación, acertar que este método debe devolver ``true``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Autor:
            getters:
                paseLegal:
                    - True: { message: "La contraseña no debe coincidir con tu nombre" }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Autor">
            <getter property="paseLegal">
                <constraint name="True">
                    <option name="message">La contraseña no debe coincidir con tu nombre</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\True(message = "La contraseña no debe coincidir con tu nombre")
             */
            public function isPaseLegal()
            {
                // devuelve true o false
            }
        }

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;

        class Autor
        {
            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addGetterConstraint('paseLegal', new True(array(
                    'message' => 'La contraseña no debe coincidir con tu nombre',
                )));
            }
        }

Ahora, crea el método ``isPaseLegal()`` e incluye la lógica que necesites::

    public function isPaseLegal()
    {
        return ($this->nombreDePila != $this->password);
    }

.. note::

    El ojo perspicaz se habrá dado cuenta de que el prefijo del captador (``get`` o ``is``) se omite en la asignación. Esto te permite mover la restricción a una propiedad con el mismo nombre más adelante (o viceversa) sin cambiar la lógica de validación.

.. _book-validation-validation-groups:

Validando grupos
----------------

Hasta ahora, hemos sido capaces de agregar restricciones a una clase y consultar si o no esa clase pasa todas las restricciones definidas. En algunos casos, sin embargo, tendrás que validar un objeto sólo contra *algunas* de las restricciones de esa clase. Para ello, puedes organizar cada restricción en uno o más "grupos de validación", y luego aplicar la validación contra un solo grupo de restricciones.

Por ejemplo, supongamos que tienes una clase ``usuario``, que se usa tanto cuando un usuario se registra como cuando un usuario actualiza su información de contacto más adelante::

    // src/Acme/BlogBundle/Entity/Usuario.php
    namespace Acme\BlogBundle\Entity;

    use Symfony\Component\Security\Core\User\UserInterface
    use Symfony\Component\Validator\Constraints as Assert;

    class Usuario implements UserInterface
    {
        /**
        * @Assert\Correo(groups={"alta"})
        */
        private $correo;

        /**
        * @Assert\NotBlank(groups={"alta"})
        * @Assert\MinLength(limit=7, groups={"alta"})
        */
        private $pase;

        /**
        * @Assert\MinLength(2)
        */
        private $ciudad;
    }

Con esta configuración, hay dos grupos de validación:

* ``Default`` - contiene las restricciones no asignadas a ningún otro grupo;

* ``alta`` - contiene restricciones sólo en los campos de ``correo electrónico`` y ``contraseña``.

Para decir al validador que use un grupo específico, pasa uno o más nombres de grupo como segundo argumento al método ``validate()``::

    $listaErrores = $validador->validate($autor, array('alta'));

Por supuesto, por lo general vas a trabajar con la validación indirectamente a través de la biblioteca de formularios. Para obtener información sobre cómo utilizar la validación de grupos dentro de los formularios, consulta :ref:`book-forms-validation-groups`.

Consideraciones finales
-----------------------

El ``validador`` de Symfony2 es una herramienta poderosa que puedes aprovechar para garantizar que los datos de cualquier objeto son "válidos". El poder detrás de la validación radica en las "restricciones", las cuales son reglas que se pueden aplicar a propiedades o métodos captadores de tu objeto. Y mientras más utilices la plataforma de validación indirectamente cuando uses formularios, recordarás que puedes utilizarla en cualquier lugar para validar cualquier objeto.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _especificación JSR303 Bean Validation:   http://jcp.org/en/jsr/detail?id=303
