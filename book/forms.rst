.. index::
   single: Formularios

Formularios
===========

Utilizar formularios HTML es una de las más comunes - y desafiantes - tareas de un desarrollador web. Symfony2 integra un componente ``Form`` que se ocupa de facilitarnos la utilización de formularios. En este capítulo, construirás un formulario complejo desde el principio, del cual, de paso, aprenderás las características más importantes de la biblioteca de formularios.

.. note::

   El componente ``Form`` de Symfony es una biblioteca independiente que se puede utilizar fuera de los proyectos Symfony2. Para más información, consulta el `Componente Form de Symfony2`_ en Github.

.. index::
   single: Formularios; Creando un formulario simple

Creando un formulario simple
----------------------------

Supongamos que estás construyendo una aplicación de tienda sencilla que necesita mostrar sus productos. Debido a que tus usuarios tendrán que editar y crear productos, tienes que crear un formulario. Pero antes de empezar, vamos a concentrarnos en la clase genérica ``Producto`` que representa y almacena los datos para un solo producto:

.. code-block:: php

    // src/Acme/TiendaBundle/Entity/Producto.php
    namespace Acme\TiendaBundle\Entity;

    class Producto
    {
        public $nombre;

        protected $precio;

        public function getPrecio()
        {
            return $this->precio;
        }

        public function setPrecio($precio)
        {
            $this->precio = $precio;
        }
    }

.. note::

   Si estás codificando este ejemplo, asegúrate de crear y habilitar el ``AcmeTiendaBundle``. Ejecuta la siguiente orden y sigue las instrucciones que aparecen en pantalla:

   .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/TiendaBundle

Este tipo de clase se denomina "antiguo objeto PHP simple", porque, hasta ahora, no tiene nada que ver con Symfony o cualquier otra biblioteca. Es simplemente un objeto PHP normal que directamente resuelve un problema dentro de *tu* aplicación (es decir, la necesidad de representar un producto en tu aplicación). Por supuesto, al final de este capítulo, serás capaz de enviar datos a una instancia de ``Producto`` (a través de un formulario), validar sus datos, y persistirlo en una base de datos.

Hasta ahora, no hemos hecho ningún trabajo relacionado con "formularios" - simplemente hemos creado una clase PHP que te ayudará a resolver un problema en *tu* aplicación.
El objetivo de construir un formulario en este capítulo es el de permitir a tus usuarios interactuar con los datos de un objeto ``Producto``.

.. index::
   single: Formularios; Creando un formulario en un controlador

Construyendo el formulario
~~~~~~~~~~~~~~~~~~~~~~~~~~

Ahora que haz creado una clase ``Producto``, el siguiente paso es crear y reproducir el formulario HTML real. En Symfony2, esto se hace construyendo un objeto ``Form`` y luego reproduciéndolo en una plantilla. Todo esto se puede hacer en el interior de un controlador:

.. code-block:: php

    // src/Acme/TiendaBundle/Controller/DefaultController.php
    namespace Acme\TiendaBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Acme\TiendaBundle\Entity\Producto;
    use Symfony\Component\HttpFoundation\Request;

    class DefaultController extends Controller
    {
        public function indexAction(Request $peticion)
        {
            // crea un producto y le da algunos datos para este ejemplo
            $producto = new Producto();
            $producto->nombre = 'Producto de prueba';
            $producto->setPrecio('50.00');

            $formulario = $this->createFormBuilder($producto)
                ->add('nombre', 'text')
                ->add('precio', 'money', array('currency' => 'USD'))
                ->getForm();

            return $this->render('AcmeTiendaBundle:Default:index.html.twig', array(
                'form' => $formulario->createView(),
            ));
        }
    }

.. tip::

   Este ejemplo muestra cómo crear el formulario directamente en el controlador.
   Más tarde, en la sección ":ref:`book-form-creating-form-classes`", aprenderás cómo construir tu formulario en una clase independiente, lo cual es muy recomendable puesto que vuelve reutilizable tu formulario.

La creación de un formulario es fácil y rápida en Symfony2 porque los objetos ``Form`` se construyen a través de un "constructor de formulario". Un constructor de formulario es un objeto con el que puedes interactuar para ayudarte a crear fácilmente objetos formulario.

En este ejemplo, hemos añadido dos campos al formulario - ``nombre`` y ``precio`` - que corresponden a las propiedades ``nombre`` y ``precio`` de la clase ``Producto``.
El campo ``nombre`` tiene un tipo de ``texto``, es decir, el usuario deberá enviar texto simple para este campo. El campo ``precio`` tiene el tipo ``money``, el cual es un campo de ``texto`` especial donde el dinero se puede mostrar y reproducir en un formato localizado. Symfony2 viene con muchos tipos integrados que explicaremos en breve (consulta :ref:`book-forms-type-reference`).

Ahora que hemos creado el formulario, el siguiente paso es reproducirlo. Lo puedes hacer fácilmente pasando un objeto formulario especial, ``vista``, a tu plantilla (consulta el ``$formulario->createView()`` en el controlador de arriba) y utilizando un conjunto de funciones ayudantes de formulario:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TiendaBundle/Resources/views/Default/index.html.twig #}

        <form action="{{ path('guarda_producto') }}" method="post" {{ form_enctype(form) }}>
            {{ form_widget(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <!-- src/Acme/TiendaBundle/Resources/views/Default/index.html.php -->

        <form action="<?php echo $view['router']->generate('guarda_producto') ?>" method="post" <?php echo $view['form']->enctype($formulario) ?> >
            <?php echo $view['form']->widget($formulario) ?>

            <input type="submit" />
        </form>

.. image:: /book/images/forms-simple.png
    :align: center

.. note::

    En este ejemplo asumimos que haz creado una ruta llamada ``guarda_producto`` que apunta al controlador ``AcmeTiendaBundle:Default:index`` creado anteriormente.

¡Eso es todo! Para imprimir ``form_widget(form)``, reproduce cada campo del formulario, junto con una etiqueta y eventuales mensajes de error. Tan fácil como esto, no es muy flexible (todavía). Por lo general, querrás reproducir cada campo individual del formulario por lo que puedes controlar la apariencia del formulario. Aprenderás cómo hacerlo en la sección ":ref:`form-rendering-template`".

Antes de continuar, observa cómo el campo de entrada ``nombre`` reproducido tiene el valor de la propiedad ``nombre`` del objeto ``$producto`` (es decir, "Producto de prueba").
El primer trabajo de un formulario es: tomar datos de un objeto y traducirlos a un formato idóneo para reproducirlos en un formulario HTML.

.. tip::

   El sistema de formularios es lo suficientemente inteligente como para acceder al valor de la propiedad protegida ``precio`` a través de los métodos ``getPrecio()`` y ``setPrecio()`` de la clase ``Producto``. A menos que una propiedad sea pública, *debe* tener métodos "captadores" y "definidores" para que el componente ``Form`` pueda obtener y fijar datos en la propiedad. Para una propiedad booleana, puedes utilizar un método "isser" (por "es servicio" por ejemplo, ``isPublicado()``) en lugar de un captador (por ejemplo, ``getPublicado()``).

.. index::
  single: Formularios; Procesando el envío del formulario

Procesando el envío de formularios
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El segundo trabajo de un ``Form`` es traducir los datos enviados por el usuario a las propiedades de un objeto. Para lograrlo, los datos presentados por el usuario deben estar vinculados al formulario. Añade las siguientes funciones en tu controlador:

.. code-block:: php

    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function indexAction(Request $peticion)
    {
        // sólo configura un nuevo objeto $producto (sin datos ficticios)
        $producto = new Producto();

        $formulario = $this->createFormBuilder($producto)
            ->add('nombre', 'text')
            ->add('precio', 'money', array('currency' => 'USD'))
            ->getForm();

        if ($peticion->getMethod() == 'POST') {
            $formulario->bindRequest($peticion);

            if ($formulario->isValid()) {
                // realiza alguna acción, tal como guardar el objeto en la base de datos

                return $this->redirect($this->generateUrl('exito_guarda_producto'));
            }
        }

        // ...
    }

Ahora, al presentar el formulario, el controlador vincula los datos presentados al formulario, el cual los traduce de nuevo en las propiedades ``nombre`` y ``precio`` del objeto ``$producto``. Todo esto ocurre a través del método ``bindRequest()``.

.. note::

    Tan pronto como se llama a ``bindRequest()``, los datos presentados se transfieren inmediatamente al objeto subyacente. Esto ocurre independientemente de si los datos subyacentes son válidos realmente.

Este controlador sigue un patrón común para el manejo de formularios, y tiene tres posibles rutas:

#. Cuando inicialmente se carga la página en un navegador, el método de la petición es ``GET``, lo cual significa que el controlador simplemente crea y reproduce el formulario (pero no lo vincula);

#. Cuando el usuario envía el formulario (es decir, el método es ``POST``), pero los datos presentados no son válidos (la validación se trata en la siguiente sección), el formulario es vinculado y, a continuación reproducido, esta vez mostrando todos los errores de validación;

#. Cuando el usuario envía el formulario con datos válidos, el formulario es vinculado y en ese momento tienes la oportunidad de realizar algunas acciones usando el objeto ``$producto`` (por ejemplo, persistirlo a la base de datos) antes de redirigir al usuario a otra página (por ejemplo, una página de "agradecimiento" o "éxito").

.. note::

   Redirigir a un usuario después de un envío de formularios con éxito evita que el usuario pueda hacer clic en "actualizar" y volver a enviar los datos.

.. index::
   single: Formularios; Validación

Validando formularios
---------------------

En la sección anterior, aprendiste cómo se puede presentar un formulario con datos válidos o no válidos. En Symfony2, la validación se aplica al objeto subyacente (por ejemplo, ``Producto``). En otras palabras, la cuestión no es si el "formulario" es válido, sino más bien si el objeto ``$producto`` es válido después de aplicarle los datos enviados en el formulario. Invocar a ``$formulario->isValid()`` es un atajo que pregunta al objeto ``$producto`` si tiene datos válidos o no.

La validación se realiza añadiendo un conjunto de reglas (llamadas restricciones) a una clase. Para ver esto en acción, añade restricciones de validación para que el campo ``nombre`` no pueda estar vacío y el campo ``precio`` no pueda estar vacío y debe ser un número no negativo:

.. configuration-block::

    .. code-block:: yaml

        # Acme/TiendaBundle/Resources/config/validation.yml
        Acme\TiendaBundle\Entity\Producto:
            properties:
                name:
                    - NotBlank: ~
                precio:
                    - NotBlank: ~
                    - Min: 0

    .. code-block:: xml

        <!-- Acme/TiendaBundle/Resources/config/validation.xml -->
        <class name="Acme\TiendaBundle\Entity\Producto">
            <property name="nombre">
                <constraint name="NotBlank" />
            </property>
            <property name="precio">
                <constraint name="Min">
                    <value>0</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Acme/TiendaBundle/Entity/Producto.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Producto
        {
            /**
             * @Assert\NotBlank()
             */
            public $nombre;

            /**
             * @Assert\NotBlank()
             * @Assert\Min(0)
             */
            protected $precio;
        }

    .. code-block:: php

        // Acme/TiendaBundle/Entity/Producto.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Min;

        class Producto
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('nombre', new NotBlank());

                $metadatos->addPropertyConstraint('precio', new NotBlank());
                $metadatos->addPropertyConstraint('precio', new Min(0));
            }
        }

¡Eso es todo! Si vuelves a enviar el formulario con datos no válidos, verás replicados los errores correspondientes en el formulario.

.. note::

   Si echas un vistazo al código HTML generado, el componente ``Form`` genera nuevos campos HTML 5 incluyendo el atributo ``required`` para forzar alguna validación directamente vía el navegador web. Algunos navegadores modernos como Firefox 4, Chrome 3.0 u Opera 9.5 entienden el atributo especial ``required``.

La validación es una característica muy poderosa de Symfony2 y tiene su propio :doc:`capítulo dedicado </book/validation>`.

.. index::
   single: Formularios; Validación de grupos

.. _book-forms-validation-groups:

Validando grupos
~~~~~~~~~~~~~~~~

Si tu objeto aprovecha la :ref:`validación de grupos <book-validation-validation-groups>`, tendrás que especificar la validación de grupos que utiliza tu formulario::

    $formulario = $this->createFormBuilder($usuarios, array(
            'validation_groups' => array('alta')
        )
        // ...
    ;

Si vas a crear :ref:`clases form <book-form-creating-form-classes>`(una buena práctica), entonces tendrás que agregar lo siguiente al método ``getDefaultOptions()``::

    public function getDefaultOptions(array $opciones) {
        return array(
            'validation_groups' => array('alta')
        );
    }

En ambos casos, *sólo* se utilizará el grupo de validación ``registro`` para validar el objeto subyacente.

.. index::
   single: Formularios; Tipos de campo integrados

.. _book-forms-type-reference:

Tipos de campo integrados
-------------------------

Symfony estándar viene con un gran grupo de tipos de campo que cubre todos los campos de formulario comunes y tipos de datos necesarios:

.. include:: /reference/forms/types/map.rst.inc

Por supuesto, también puedes crear tus propios tipos de campo personalizados. Este tema se trata en el artículo ":doc:`/cookbook/forms/create_custom_field_type`" del recetario.

.. index::
   single: Formularios; Opciones de tipo de campo

Opciones comunes de tipo de campo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Posiblemente habrás notado que al campo ``precio`` se le ha pasado un arreglo de opciones:

.. code-block:: php

    ->add('precio', 'money', array('currency' => 'USD'))

Cada tipo de campo tiene una diferente serie de opciones que le puedes pasar.
Muchas de ellas son específicas para el tipo de campo y puedes encontrar los detalles en la documentación de cada tipo. Algunas opciones, sin embargo, se comparten entre la mayoría de los campos:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/max_length.rst.inc

.. index::
   single: Formularios; Adivinando el tipo de campo

.. _book-forms-field-guessing:

Adivinando el tipo de campo
---------------------------

Ahora que haz añadido metadatos de validación a la clase ``Producto``, Symfony ya sabe un poco sobre tus campos. Si le permites, Symfony puede "adivinar" el tipo de tu campo y configurarlo por ti. En este ejemplo, Symfony adivinará las reglas de validación para ambos campos ``nombre`` y ``precio`` como campos de ``texto`` normales. Debido a que estámos sobre el campo ``nombre``, puedes modificar tu código para que Symfony adivine el campo para ti:

.. code-block:: php

    public function indexAction()
    {
        $producto = new Producto();

        $formulario = $this->createFormBuilder($producto)
            ->add('nombre')
            ->add('precio', 'money', array('currency' => 'USD'))
            ->getForm();
    }

Ahora hemos omitido el tipo ``text`` para el campo ``nombre``, ya que será adivinado correctamente por las reglas de validación. Sin embargo, mantuvimos el tipo ``money`` del campo ``precio``, ya que es más específico de lo que el sistema podría adivinar (``text``).

.. note::

    El método ``createFormBuilder()`` toma hasta dos argumentos ninguno de los cuales es obligatorio):

     * Los datos predeterminados para iniciar los campos del formulario. Este argumento puede ser una matriz asociativa o un antiguo objeto PHP simple como en este ejemplo;

     * una matriz de opciones para el formulario.

Este ejemplo es bastante trivial, pero el campo se puede adivinar ahorrando tiempo importante.
Como verás más adelante, agregar metadatos Doctrine puede mejorar notablemente la capacidad del sistema para adivinar los tipos de campo.

.. caution::

    El adivino basado en restricciones de validación no tiene en cuenta la validación de grupos. Si las opciones del adivino no son correctas, siempre las puedes modificar como mejor te parezca.

.. index::
   single: Formularios; Reproduciendo en una plantilla

.. _form-rendering-template:

Reproduciendo un formulario en una plantilla
--------------------------------------------

Hasta ahora, haz visto cómo se puede reproducir todo el formulario con una sola línea de código. Por supuesto, generalmente necesitarás mucha más flexibilidad al reproducirlo:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TiendaBundle/Resources/views/Default/index.html.twig #}

        <form action="{{ path('guarda_producto') }}" method="post" {{ form_enctype(form) }}>
            {{ form_errors(form) }}

            {{ form_row(form.nombre) }}
            {{ form_row(form.precio) }}

            {{ form_rest(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <!-- // src/Acme/TiendaBundle/Resources/views/Default/index.html.php -->

        <form action="<?php echo $view['router']->generate('guarda_producto') ?>" method="post" <?php echo $view['form']->enctype($formulario) ?>>
            <?php echo $view['form']->errors($formulario) ?>

            <?php echo $view['form']->row($formulario['nombre']) ?>
            <?php echo $view['form']->row($formulario['precio']) ?>

            <?php echo $view['form']->rest($formulario) ?>

            <input type="submit" />
        </form>

Echemos un vistazo a cada parte:

* ``form_enctype(form)`` - Si por lo menos un campo es un campo de carga de archivos, se reproduce el obligado ``enctype="multipart/form-data"``;

* ``form_errors(form)`` - Reproduce cualquier error global para todo el formulario (los errores específicos de campo se muestran junto a cada campo);

* ``form_row(form.precio)`` - Reproduce la etiqueta, cualquier error, y el elemento gráfico de formulario HTML para el campo dado (por ejemplo, ``precio``);

* ``form_rest(form)`` - Regenera todos los campos que aún no se han reproducido.
  Por lo general es buena idea realizar una llamada a este ayudante en la parte inferior de cada formulario (en caso de haber olvidado sacar un campo o si no quieres preocuparte de reproducir manualmente los campos ocultos). Este ayudante también es útil para tomar ventaja de la :ref:`Protección CSRF <forms-csrf>` automática.

La mayor parte del trabajo la realiza el ayudante ``form_row``, el cual de manera predeterminada reproduce la etiqueta, los errores y el elemento gráfico HTML de cada campo del formulario dentro de una etiqueta ``div``. En la sección :ref:`form-theming`, aprenderás cómo se puede personalizar ``form_row`` en muchos niveles diferentes.

.. tip::

    A partir de HTML5, los navegadores pueden validar "restricciones" del formulario interactivamente.
    Los formularios generados sacan el máximo provecho de esta nueva característica añadiendo razonables atributos HTML. No obstante, se puede desactivar utilizando el atributo ``novalidate`` en la etiqueta ``form`` o ``formnovalidate`` en la etiqueta ``submit``.

Reproduciendo cada campo a mano
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El ayudante ``form_row`` es magnífico porque rápidamente puedes reproducir cada campo del formulario (y también puedes personalizar el formato utilizado para la "fila"). Pero puesto que la vida no siempre es tan simple, también puedes reproducir cada campo completamente a mano:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_errors(form) }}

        <div>
            {{ form_label(form.nombre) }}
            {{ form_errors(form.nombre) }}
            {{ form_widget(form.nombre) }}
        </div>

        <div>
            {{ form_label(form.precio) }}
            {{ form_errors(form.precio) }}
            {{ form_widget(form.precio) }}
        </div>

        {{ form_rest(form) }}

    .. code-block:: html+php

        <?php echo $view['form']->errors($formulario) ?>

        <div>
            <?php echo $view['form']->label($formulario['nombre']) ?>
            <?php echo $view['form']->errors($formulario['nombre']) ?>
            <?php echo $view['form']->widget($formulario['nombre']) ?>
        </div>

        <div>
            <?php echo $view['form']->label($formulario['precio']) ?>
            <?php echo $view['form']->errors($formulario['precio']) ?>
            <?php echo $view['form']->widget($formulario['precio']) ?>
        </div>

        <?php echo $view['form']->rest($formulario) ?>

Si la etiqueta generada automáticamente para un campo no es del todo correcta, la puedes especificar explícitamente:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.nombre, 'Nombre producto') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($formulario['nombre'], 'Nombre producto') ?>

Por último, algunos tipos de campo tienen más opciones para su representación que puedes pasar al elemento gráfico. Estas opciones están documentadas con cada tipo, pero una de las opciones común es ``attr``, la cual te permite modificar los atributos en el elemento del formulario.
A continuación debes añadir la clase ``name_field`` al campo de entrada de texto reproducido:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.nombre, { 'attr': {'class': 'name_field'} }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($formulario['nombre'], array(
            'attr' => array('class' => 'name_field'),
        )) ?>

Referencia de funciones de plantilla Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si estás utilizando Twig, hay disponible una referencia completa de las funciones de reproducción de formularios en el :doc:`Manual de referencia </reference/forms/twig_reference>`.

.. index::
   single: Formularios; Creando clases ``Form``

.. _book-form-creating-form-classes:

Creando clases ``Form``
-----------------------

Como hemos visto, puedes crear un formulario y utilizarlo directamente en un controlador.
Sin embargo, una mejor práctica es construir el formulario en una clase separada, independiente de las clases PHP, la cual puedes reutilizar en cualquier lugar de tu aplicación. Crea una nueva clase que albergará la lógica para construir el formulario del producto:

.. code-block:: php

    // src/Acme/TiendaBundle/Form/Type/ProductoType.php

    namespace Acme\TiendaBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class ProductoType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $opciones)
        {
            $builder->add('nombre');
            $builder->add('precio', 'money', array('currency' => 'USD'));
        }

        public function getNombre()
        {
            return 'producto';
        }
    }

Esta nueva clase contiene todas las instrucciones necesarias para crear el formulario del producto (ten en cuenta que el método ``getNombre()`` debe devolver un identificador único para el tipo). La puedes utilizar para construir rápidamente un objeto formulario en el controlador:

.. code-block:: php

    // src/Acme/TiendaBundle/Controller/DefaultController.php

    // añade esta nueva declaración use en lo alto de la clase
    use Acme\TiendaBundle\Form\Type\ProductoType;

    public function indexAction()
    {
        $producto = // ...
        $formulario = $this->createForm(new ProductoType(), $producto);

        // ...
    }

.. note::
    También puedes establecer los datos en el formulario a través del método ``setData()``:

    ..

    .. code-block:: php

        $formulario = $this->createForm(new ProductoType());
        $formulario->setData($producto);

    Si utilizas el método ``setData`` - y quieres tomar ventaja de el adivinado del tipo de campo, asegúrate de agregar lo siguiente a tu clase formulario:

    .. code-block:: php

        public function getDefaultOptions(array $opciones)
        {
            return array(
                'data_class' => 'Acme\TiendaBundle\Entity\Producto',
            );
        }

    Esto es necesario porque el objeto se pasa al formulario después de adivinar el tipo de campo.

Colocar la lógica del formulario en su propia clase significa que fácilmente puedes reutilizar el formulario en otra parte del proyecto. Esta es la mejor manera de crear formularios, pero la decisión en última instancia, depende de ti.

.. index::
   pair: Formularios; Doctrine

Formularios y Doctrine
----------------------

El objetivo de un formulario es traducir los datos a un objeto (por ejemplo, ``Producto``) a un formulario HTML y luego traducir los datos enviados por el usuario al objeto original. Como tal, el tema de la persistencia del objeto ``Producto`` a la base de datos es del todo ajeno al tema de los formularios. Si haz configurado la clase ``Producto`` para persistirlo con Doctrine, entonces lo puedes persistir después de un envío del formulario cuando el formulario es válido:

.. code-block:: php

    if ($formulario->isValid()) {
        $em = $this->get('doctrine')->getEntityManager();
        $em->persist($producto);
        $em->flush();

        return $this->redirect($this->generateUrl('exito_guarda_producto'));
    }

Si por alguna razón, no tienes acceso a tu objeto ``$producto`` original, lo puedes recuperar dsde el formulario:

.. code-block:: php

    $producto = $formulario->getData();

Para más información, consulta el capítulo :doc:`ORM de Doctrine </book/doctrine>`.

La clave es entender que cuando el formulario está vinculado, los datos presentados inmediatamente se transfieren al objeto subyacente. Si deseas conservar los datos, sólo tendrás que conservar el objeto en sí (el cual ya contiene los datos presentados).

Si el objeto subyacente de un formulario (por ejemplo, ``Producto``) pasa a ser asignado con el ORM de Doctrine, la plataforma de formularios utiliza esa información - junto con los metadatos de validación - para adivinar el tipo de un campo en particular.

.. index::
   single: Formularios; Integrando formularios

Integrando formularios
----------------------

A menudo, querrás crear un formulario que incluye campos de muchos objetos diferentes. Por ejemplo, un formulario de registro puede contener datos que pertenecen a un objeto ``Usuario``, así como a muchos objetos ``Domicilio``. Afortunadamente, esto es fácil y natural con el componente ``Form``.

Integrando un solo objeto
~~~~~~~~~~~~~~~~~~~~~~~~~

Supongamos que cada ``Producto`` pertenece a un simple objeto ``Categoría``:

.. code-block:: php

    // src/Acme/TiendaBundle/Entity/Categoria.php
    namespace Acme\TiendaBundle\Entity;

    use Symfony\Component\Validator\Constraints as Assert;

    class Categoria
    {
        /**
         * @Assert\NotBlank()
         */
        public $nombre;
    }

La clase ``Producto`` tiene una nueva propiedad ``$categoria``, indicando a qué ``Categoría`` pertenece:

.. code-block:: php

    use Symfony\Component\Validator\Constraints as Assert;

    class Producto
    {
        // ...

        /**
         * @Assert\Type(type="Acme\TiendaBundle\Entity\Categoria")
         */
        protected $categoria;

        // ...

        public function getCategoria()
        {
            return $this->categoria;
        }

        public function setCategoria(Categoria $categoria)
        {
            $this->categoria = $categoria;
        }
    }

Ahora que hemos actualizado tu aplicación para reflejar los nuevos requisitos, crea una clase ``Form`` para que el usuario pueda modificar un objeto ``Categoria``:

.. code-block:: php

    // src/Acme/TiendaBundle/Form/Type/CategoriaType.php
    namespace Acme\TiendaBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class CategoriaType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $opciones)
        {
            $builder->add('nombre');
        }

        public function getDefaultOptions(array $opciones)
        {
            return array(
                'data_class' => 'Acme\TiendaBundle\Entity\Categoria',
            );
        }

        public function getNombre()
        {
            return 'categoria';
        }
    }

El tipo del campo ``nombre`` se adivina (como un campo ``texto``) a partir de los metadatos de validación del objeto ``Categoria``.

El objetivo final es permitir que la ``Categoria`` de un ``Producto`` sea modificada sólo al interior del formulario del producto. Para lograrlo, añade un campo ``categoria`` al objeto ``ProductoType`` cuyo tipo es una instancia de la nueva clase ``CategoriaType``:

.. code-block:: php

    public function buildForm(FormBuilder $builder, array $opciones)
    {
        // ...

        $builder->add('categoria', new CategoriaType());
    }

Ahora puedes reproducir los campos de ``CategoriaType`` junto a los de la clase ``ProductoType``. Reproduce los campos de ``Categoria`` en la misma forma que los campos del ``Producto`` original:

.. configuration-block::

    .. code-block:: html+jinja

        {# ... #}
        {{ form_row(form.precio) }}

        <h3>Categoría</h3>
        <div class="categoria">
            {{ form_row(form.categoria.nombre) }}
        </div>

        {{ form_rest(form) }}
        {# ... #}

    .. code-block:: html+php

        <!-- ... -->
        <?php echo $view['form']->row($formulario['precio']) ?>

        <h3>Categoría</h3>
        <div class="categoria">
            <?php echo $view['form']->row($formulario['categoria']['nombre']) ?>
        </div>

        <?php echo $view['form']->rest($formulario) ?>
        <!-- ... -->

Cuando el usuario envía el formulario, los datos presentados para los campos de ``Categoria`` se combina en el objeto ``Categoria``. En otras palabras, todo funciona exactamente como lo hace el objeto ``Producto`` principal. La instancia de ``Categoría`` es accesible naturalmente vía ``$producto->getCategoria()`` y la puedes persistir en la base de datos o utilizarla como necesites.

Integrando una colección de formularios
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

También puedes integrar una colección de formularios en un formulario. Esto se consigue usando el tipo de campo ``collection``. Asumiendo que tienes una propiedad llamada ``revisiones`` y una clase llamada ``RevisionProductoType``, podrías hacer lo siguiente dentro de ``ProductoType``:

.. code-block:: php

    public function buildForm(FormBuilder $builder, array $opciones)
    {
        // ...

        $builder->add('revisiones', 'collection', array(
           'type' => new RevisionProductoType(),
        ));
    }

.. index::
   single: Formularios; Tematizando
   single: Formularios; Campos personalizados

.. _form-theming:

Tematizando formularios
-----------------------

Puedes personalizar cómo se reproduce cada parte de un formulario. Eres libre de cambiar la forma en que se reproduce cada "fila" del formulario, cambiar el formato que sirve para reproducir errores, e incluso personalizar la forma en que se debe reproducir una etiqueta textarea. Nada está fuera de límites, y puedes usar diferentes personalizaciones en diferentes lugares.

Symfony utiliza plantillas para reproducir todo y cada parte de un formulario, tal como etiquetas de campo ``label``, etiquetas ``input``, mensajes de error y todo lo demás.

En Twig, cada "fragmento" del formulario está representado por un bloque Twig. Para personalizar alguna parte de cómo se reproduce un formulario, sólo hay que reemplazar el bloque adecuado.

En PHP, cada "fragmento" del formulario se reproduce vía un archivo de plantilla individual.
Para personalizar cualquier parte de cómo se reproduce un formulario, sólo hay que reemplazar la plantilla existente creando una nueva.

Para entender cómo funciona esto, vamos a personalizar el fragmento ``form_row`` añadiendo un atributo de clase al elemento ``div`` que rodea cada fila. Para ello, crea un nuevo archivo de plantilla que almacenará el nuevo marcado:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TiendaBundle/Resources/views/Form/fields.html.twig #}

        {% block field_row %}
        {% spaceless %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endspaceless %}
        {% endblock field_row %}

    .. code-block:: html+php

        <!-- src/Acme/TiendaBundle/Resources/views/Form/field_row.html.php -->

        <div class="form_row">
            <?php echo $view['form']->label($formulario, $label) ?>
            <?php echo $view['form']->errors($formulario) ?>
            <?php echo $view['form']->widget($formulario, $parameters) ?>
        </div>

El fragmento ``field_row`` del formulario se usa cuando se reproduce la mayoría de los campos a través de la función ``form_row``. Para decir al componente que utilice tu nuevo fragmento ``field_row`` definido anteriormente, añade lo siguiente en la parte superior de la plantilla que reproduce el formulario:

.. configuration-block:: php

    .. code-block:: html+jinja

        {# src/Acme/TiendaBundle/Resources/views/Default/index.html.twig #}

        {% form_theme form 'AcmeTiendaBundle:Form:fields.html.twig' %}

        <form ...>

    .. code-block:: html+php

        <!-- src/Acme/TiendaBundle/Resources/views/Default/index.html.php -->

        <?php $view['form']->setTheme($formulario, array('AcmeTiendaBundle:Form')) ?>

        <form ...>

La etiqueta ``form_theme`` (en Twig) "importa" los fragmentos definidos en la plantilla dada y los utiliza al reproducir el formulario. En otras palabras, cuando se llame a ``form_row`` más adelante en esta plantilla, utilizará el bloque ``field_row`` desde tu tema personalizado.

Para personalizar cualquier parte de un formulario, sólo tienes que reemplazar el fragmento apropiado. Saber exactamente qué bloque sustituir es el tema de la siguiente sección.

En la siguiente sección, aprenderás más acerca de cómo personalizar diferentes partes de un formulario. Para una explicación más extensa, consulta :doc:`/cookbook/form/form_customization`.

.. _form-template-blocks:

Bloques de formulario en plantilla
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En Symfony, cada parte que se reproduce de un formulario - elementos HTML de formulario, errores, etiquetas, etc. - se define en un tema base, el cual es una colección de bloques en Twig y una colección de archivos de plantilla en PHP.

En Twig, todos los bloques necesarios se definen en el archivo `form_div_base.html.twig`_ que reside en el interior del `Twig Bridge`_. Dentro de este archivo, puedes ver todos los bloques necesarios para reproducir un formulario y cada tipo de campo predeterminado.

En PHP, los fragmentos son archivos de plantilla individuales. De manera predeterminada se encuentran en el directorio ``Resources/views/Form`` del paquete de la plataforma (`ver en GitHub`_).

El nombre de cada fragmento sigue el mismo patrón básico y se divide en dos partes, separadas por un solo carácter de subrayado (``_``). Algunos ejemplos son:

* ``field_row`` - usado por ``form_row`` para reproducir la mayoría de los campos;
* ``textarea_widget`` - usado por ``form_widget`` para reproducir un campo de tipo ``textarea``;
* ``field_errors`` - usado por ``form_errors`` para reproducir los errores de un campo;

Cada fragmento sigue el mismo patrón básico: ``tipo_parte``. La porción ``tipo`` corresponde con el tipo del campo que se está reproduciendo (por ejemplo, ``textarea`` o ``checkbox``), mientras que la porción ``parte`` corresponde a *qué* se está reproduciendo (por ejemplo, ``label``, ``widget``). De forma predeterminada, hay exactamente siete partes posibles que un formulario puede reproducir:

+-------------+----------------------------+------------------------------------------------------+
| ``label``   | (p. ej. ``field_label``)   | reproduce la etiqueta del campo                      |
+-------------+----------------------------+------------------------------------------------------+
| ``widget``  | (p. ej. ``field_widget``)  | reproduce el elemento gráfico HTML del campo         |
+-------------+----------------------------+------------------------------------------------------+
| ``errors``  | (p. ej. ``field_errors``)  | reproduce los errores del campo                      |
+-------------+----------------------------+------------------------------------------------------+
| ``row``     | (p. ej. ``field_row``)     | reproduce la fila completa del campo                 |
|             |                            | (etiqueta+elemento gráfico+errores)                  |
+-------------+----------------------------+------------------------------------------------------+
| ``rows``    | (p. ej. ``field_rows``)    | reproduce las filas hijas de un formulario           |
+-------------+----------------------------+------------------------------------------------------+
| ``rest``    | (p. ej. ``field_rest``)    | reproduce los campos sin procesar del formulario     |
+-------------+----------------------------+------------------------------------------------------+
| ``enctype`` | (p. ej. ``field_enctype``) | reproduce el atributo ``enctype`` del formulario     |
+-------------+----------------------------+------------------------------------------------------+

Al conocer el tipo de campo (por ejemplo, ``textarea``) y cual parte deseas personalizar (por ejemplo, ``widget``), puedes construir el nombre del fragmento que se debe redefinir (por ejemplo, ``textarea_widget``).

Heredando el tipo de bloque en formulario
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En algunos casos, parece que falta el fragmento que deseas personalizar.
Por ejemplo, no hay fragmento ``textarea_errors`` en los temas predeterminados provistos con Symfony. Entonces, ¿cómo se reproducen los errores de un campo ``textarea``?

La respuesta es: a través del fragmento ``field_errors``. Cuando Symfony reproduce los errores del tipo ``textarea``, primero busca un fragmento ``textarea_errors`` antes de caer de nuevo al fragmento ``field_errors``. Cada tipo de campo tiene un tipo *padre* (el tipo primario del ``textarea`` es ``field``), y Symfony utiliza el fragmento para el tipo del padre si no existe el fragmento base.

Por lo tanto, para sustituir *sólo* los errores de los campos ``textarea``, copia el fragmento ``field_errors``, cámbiale el nombre a ``textarea_errors`` y personalizarlo. Para sustituir la reproducción predeterminada para error de *todos* los campos, copia y personaliza el fragmento ``field_errors`` directamente.

Tematizando formularios globalmente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Además de *tematizar* plantillas individuales, también puedes decir a Symfony que importe personalizaciones de formulario a través de tu proyecto entero.

Twig
....

Cuando utilizas Twig, hemos visto cómo puedes utilizar la etiqueta ``form_theme`` de Twig en una plantilla para importar personalizaciones de formulario que se deben utilizar en esa plantilla.
También puedes decirle a Symfony que utilice automáticamente ciertas personalizaciones de formulario en todas las plantillas de tu aplicación. Para incluir automáticamente los bloques de plantilla personalizados ``fields.html.twig`` creados anteriormente, modifica el archivo de configuración de tu aplicación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeTiendaBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <twig:config ...>
                <twig:form>
                    <resource>AcmeTiendaBundle:Form:fields.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php

        $contenedor->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'AcmeTiendaBundle:Form:fields.html.twig',
             ))
            // ...
        ));

Ahora se utilizan todos los bloques dentro de la plantilla ``fields.html.twig`` a nivel global para definir el formulario producido.

.. sidebar::  Personalizando todo el resultado del formulario en un único archivo Twig

    También puedes personalizar un sólo bloque del formulario dentro de la plantilla donde necesites tal personalización.

    .. code-block:: html+jinja

        {% extends '::base.html.twig' %}

        {% form_theme form _self %}

        {% block field_row %}
            {# produce la fila personalizada de un campo #}
        {% endblock field_row %}

        {% block contenido %}
            {# ... #}

            {{ form_row(form.nombre) }}
        {% endblock %}

    La etiqueta ``{% form_theme form_self %}`` permite personalizar bloques directamente dentro de la plantilla que utilizará las personalizaciones. Utiliza este método para crear rápidamente formularios personalizados que sólo son necesarios en una sola plantilla.

    Los bloques ``Form`` definidos en los recursos de la extensión (`form_div_base.html.twig`_) y los temas en las vistas padre son accesibles desde un bloque ``Form``. Esta característica se muestra en la personalización del siguiente formulario que utiliza el bloque ``atributos`` definido en `form_div_base.html.twig`_:

    .. code-block:: html+jinja

        {% block text_widget %}
            <div class="text_widget">
                <input type="text" {{ block('widget_attributes') }} value="{{ value }}" />
            </div>
        {% endblock %}

PHP
...

Cuando usas PHP, haz visto cómo se puede utilizar el método ayudante ``setTheme`` en una plantilla para importar personalizaciones de formulario que se utilizan dentro de esa plantilla.
También puedes decirle a Symfony que utilice automáticamente ciertas personalizaciones de formulario en todas las plantillas de tu aplicación. Para incluir automáticamente las plantillas personalizadas del directorio `Acme/TiendaBundle/Resources/views/Form` creado anteriormente, modifica el archivo de configuración de tu aplicación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        framework:
            templating:
                form:
                    resources:
                        - 'AcmeTiendaBundle:Form'
        # ...


    .. code-block:: xml

        <!-- app/config/config.xml -->

        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>AcmeTiendaBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php

        $contenedor->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'AcmeTiendaBundle:Form',
             )))
            // ...
        ));

Cualquier fragmento dentro del directorio `Acme/TiendaBundle/Resources/views/Form` ahora se utiliza globalmente para definir la salida del formulario.

.. index::
   single: Formularios; Protección CSRF

.. _forms-csrf:

Protección CSRF
---------------

CSRF (Cross-site request forgery) - o `Falsificación de petición en sitios cruzados`_  - es un método por el cual un usuario malintencionado intenta hacer que tus usuarios legítimos, sin saberlo, presenten datos que no tienen la intención de enviar. Afortunadamente, los ataques CSRF se pueden prevenir usando un elemento CSRF dentro de tus formularios.

La buena nueva es que, por omisión, Symfony integra y valida elementos CSRF automáticamente. Esto significa que puedes aprovechar la protección CSRF sin hacer nada. De hecho, ¡cada formulario en este capítulo se ha aprovechado de la protección CSRF!

La protección CSRF funciona añadiendo un campo al formulario - por omisión denominado ``_token`` - que contiene un valor que sólo tú y tu usuario conocen. Esto garantiza que el usuario - y no alguna otra entidad - es el que presenta dichos datos. Symfony automáticamente valida la presencia y exactitud de este elemento.

El campo ``_token`` es un campo oculto y será reproducido automáticamente si se incluye la función ``form_rest()`` de la plantilla, la cual garantiza que se presenten todos los campos producidos.

El elemento CSRF se puede personalizar formulario por formulario. Por ejemplo:

.. code-block:: php

    class ProductoType extends AbstractType
    {
        public function getDefaultOptions(array $opciones)
        {
            return array(
                'data_class'      => 'Acme\TiendaBundle\Entity\Producto',
                'csrf_protection' => true,
                'csrf_field_nombre' => '_token',
                'intention'  => 'product_creation',
            );
        }

        public function getNombre()
        {
            return 'producto';
        }
    }

Para desactivar la protección CSRF, fija la opción ``csrf_protection`` a false.
Las personalizaciones también se pueden hacer a nivel global en tu proyecto. Para más información, consulta la sección :ref:`referencia de configuración de formularios </reference-frameworkbundle-forms>`.

.. note::

    La opción ``intention`` es opcional pero mejora considerablemente la seguridad del elemento generado produciendo uno diferente para cada formulario.

Consideraciones finales
-----------------------

Ahora ya conoces todos los bloques de construcción necesarios para construir formularios complejos y funcionales para tu aplicación. Cuando construyas formularios, ten en cuenta que el primer objetivo de un formulario es traducir los datos de un objeto (``Producto``) a un formulario HTML para que el usuario pueda modificar los datos. El segundo objetivo de un formulario es tomar los datos presentados por el usuario y volverlos a aplicar al objeto.

Todavía hay mucho más que aprender sobre el poderoso mundo de los formularios, tal como la forma de :doc:`manejar con Doctrine los archivos subidos </cookbook/doctrine/file_uploads>` o cómo crear un formulario donde puedes agregar una serie dinámica de subformularios (por ejemplo, una lista de tareas donde puedes seguir añadiendo más campos a través de Javascript antes de presentarlo). Consulta el recetario para estos temas.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/doctrine/file_uploads`
* :doc:`Referencia del campo file </reference/forms/types/file>`
* :doc:`Creando tipos de campo personalizados </cookbook/form/create_custom_field_type>`
* :doc:`/cookbook/form/form_customization`

.. _`Componente Form de Symfony2`: https://github.com/symfony/Form
.. _`Twig Bridge`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bridge/Twig
.. _`form_div_base.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_base.html.twig
.. _`Falsificación de petición en sitios cruzados`: http://es.wikipedia.org/wiki/Cross_Site_Request_Forgery _`ver en GitHub`:   https://github.com/symfony/symfony/tree/master/src/Symfony/Bundle/FrameworkBundle/Resources/views/Form
