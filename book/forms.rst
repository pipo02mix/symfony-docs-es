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

Supongamos que estás construyendo una sencilla aplicación de tareas pendientes que necesita mostrar tus "pendientes". Debido a que tus usuarios tendrán que editar y crear tareas, tienes que crear un formulario. Pero antes de empezar, vamos a concentrarnos en la clase genérica ``Tarea`` que representa y almacena los datos para una sola tarea:

.. code-block:: php

    // src/Acme/TareaBundle/Entity/Tarea.php
    namespace Acme\TareaBundle\Entity;

    class Tarea
    {
        protected $tarea;

        protected $fechaVencimiento;

        public function getTarea()
        {
            return $this->tarea;
        }
        public function setTarea($tarea)
        {
            $this->tarea = $tarea;
        }

        public function getFechaVencimiento()
        {
            return $this->fechaVencimiento;
        }
        public function setfechaVencimiento(\DateTime $fechaVencimiento = null)
        {
            $this->fechaVencimiento = $fechaVencimiento;
        }
    }

.. note::

   Si estás codificando este ejemplo, primero crea el ``AcmeTareaBundle`` ejecutando la siguiente orden (aceptando todas las opciones predeterminadas):

   .. code-block:: bash

        php app/console generate:bundle --namespace=Acme/TareaBundle

Esta clase es una "antiguo objeto PHP sencillo", ya que, hasta ahora, no tiene nada que ver con Symfony o cualquier otra biblioteca. Es simplemente un objeto PHP normal que directamente resuelve un problema dentro de *tu* aplicación (es decir, la necesidad de representar una tarea pendiente en tu aplicación). Por supuesto, al final de este capítulo, serás capaz de enviar datos a una instancia de ``Tarea`` (a través de un formulario), validar sus datos, y persistirla en una base de datos.

.. index::
   single: Formularios; Creando un formulario en un controlador

Construyendo el formulario
~~~~~~~~~~~~~~~~~~~~~~~~~~

Ahora que haz creado una clase ``Tarea``, el siguiente paso es crear y reproducir el formulario HTML real. En Symfony2, esto se hace construyendo un objeto ``Form`` y luego reproduciéndolo en una plantilla. Por ahora, esto se puede hacer en el interior de un controlador::

    // src/Acme/TareaBundle/Controller/DefaultController.php
    namespace Acme\TareaBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Acme\TareaBundle\Entity\Tarea;
    use Symfony\Component\HttpFoundation\Request;

    class DefaultController extends Controller
    {
        public function nuevaAction(Request $peticion)
        {
            // crea una tarea y proporciona algunos datos ficticios para este ejemplo
            $tarea = new Tarea();
            $tarea->setTarea('Escribir una entrada para el blog');
            $tarea->setFechaVencimiento(new \DateTime('tomorrow'));

            $form = $this->createFormBuilder($tarea)
                ->add('tarea', 'text')
                ->add('fechaVencimiento', 'date');
                ->getForm();

            return $this->render('AcmeTareaBundle:Default:new.html.twig', array(
                'form' => $form->createView(),
            ));
        }
    }

.. tip::

   Este ejemplo muestra cómo crear el formulario directamente en el controlador.
   Más tarde, en la sección ":ref:`book-form-creating-form-classes`", aprenderás cómo construir tu formulario en una clase independiente, lo cual es muy recomendable puesto que vuelve reutilizable tu formulario.

La creación de un formulario requiere poco código relativamente, porque los objetos ``form`` de Symfony2 se construyen con un "constructor de formularios". El propósito del constructor del formulario es que puedas escribir sencillas "recetas"  de formularios, y hacer todo el trabajo pesado, de hecho construye el formulario.

En este ejemplo, hemos añadido dos campos al formulario - ``tarea`` y ``fechaVencimiento`` - que corresponden a las propiedades ``tarea`` y ``fechaVencimiento`` de la clase ``Tarea``.
También haz asignado a cada uno un "tipo" (por ejemplo, ``text``, ``date``), que, entre otras cosas, determinan qué etiqueta de formulario HTML se reproduce para ese campo.

Symfony2 viene con muchos tipos integrados que explicaremos en breve (consulta :ref:`book-forms-type-reference`).

.. index::
  single: Formularios; reproducción básica de plantilla

Reproduciendo el ``Form``
~~~~~~~~~~~~~~~~~~~~~~~~~

Ahora que hemos creado el formulario, el siguiente paso es reproducirlo. Lo puedes hacer fácilmente pasando un objeto especial del formulario, ``view``, a tu plantilla (consulta el ``$form->createView()`` en el controlador de arriba) y utilizando un conjunto de funciones ayudantes de formulario:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TareaBundle/Resources/views/Default/nueva.html.twig #}

        <form action="{{ path('tarea_nueva') }}" method="post" {{ form_enctype(form) }}>
            {{ form_widget(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <!-- src/Acme/TareaBundle/Resources/views/Default/nueva.html.php -->

        <form action="<?php echo $view['router']->generate('tarea_nueva') ?>" method="post" <?php echo $view['form']->enctype($form) ?> >
            <?php echo $view['form']->widget($form) ?>

            <input type="submit" />
        </form>

.. image:: /images/book/form-simple.png
    :align: center

.. note::

    Este ejemplo asume que haz creado una ruta llamada ``tarea_nueva`` que apunta a la ``AcmeTareaBundle:Default:nueva`` controlador creado anteriormente.

¡Eso es todo! Al imprimir ``form_widget(form)``, se reproduce cada campo en el formulario, junto con la etiqueta y un mensaje de error (si lo hay). Tan fácil como esto, aunque no es muy flexible (todavía). Por lo general, querrás reproducir individualmente cada campo del formulario para que puedas controlar la apariencia del formulario. Aprenderás cómo hacerlo en la sección ":ref:`form-rendering-template`".

Antes de continuar, observa cómo el campo de entrada ``tarea`` reproducido tiene el valor de la propiedad ``tarea`` del objeto ``$tarea`` (es decir, "Escribir una entrada del blog").
El primer trabajo de un formulario es: tomar datos de un objeto y traducirlos a un formato idóneo para reproducirlos en un formulario HTML.

.. tip::

   El sistema de formularios es lo suficientemente inteligente como para acceder al valor de la propiedad protegida ``tarea`` a través de los métodos ``getTarea()`` y ``setTarea()`` de la clase ``Tarea``. A menos que una propiedad sea pública, *debe* tener métodos "captadores" y "definidores" para que el componente ``Form`` pueda obtener y fijar datos en la propiedad. Para una propiedad booleana, puedes utilizar un método "isser" (por "es servicio", por ejemplo, ``isPublicado()``) en lugar de un captador (por ejemplo, ``getPublicado()``).

.. index::
  single: Formularios; Procesando el envío del formulario

Procesando el envío de formularios
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El segundo trabajo de un ``Form`` es traducir los datos enviados por el usuario a las propiedades de un objeto. Para lograrlo, los datos presentados por el usuario deben estar vinculados al formulario. Añade las siguientes funcionalidad a tu controlador::

    // ...

    public function nuevaAction(Request $peticion)
    {
        // sólo configura un objeto $tarea fresco (remueve los datos de prueba)
        $tarea = new Tarea();

        $formulario = $this->createFormBuilder($tarea)
            ->add('tarea', 'text')
            ->add('fechaVencimiento', 'date')
            ->getForm();

        if ($peticion->getMethod() == 'POST') {
            $formulario->bindRequest($peticion);

            if ($formulario->isValid()) {
                // realiza alguna acción, tal como guardar la tarea en la base de datos

                return $this->redirect($this->generateUrl('tarea_exito'));
            }
        }

        // ...
    }

Ahora, cuando se presente el formulario, el controlador vincula al formulario los datos presentados, los cuales se traducen en los nuevos datos de las propiedades ``tarea`` y ``fechaVencimiento`` del objeto ``$tarea``. Todo esto ocurre a través del método ``bindRequest()``.

.. note::

    Tan pronto como se llama a ``bindRequest()``, los datos presentados se transfieren inmediatamente al objeto subyacente. Esto ocurre independientemente de si los datos subyacentes son válidos realmente.

Este controlador sigue un patrón común para el manejo de formularios, y tiene tres posibles rutas:

#. Inicialmente, cuando, se carga el formulario en un navegador, el método de la petición es ``GET``, lo cual significa simplemente que se debe crear y reproducir el formulario;

#. Cuando el usuario envía el formulario (es decir, el método es ``POST``), pero los datos presentados no son válidos (la validación se trata en la siguiente sección), el formulario es vinculado y, a continuación reproducido, esta vez mostrando todos los errores de validación;

#. Cuando el usuario envía el formulario con datos válidos, el formulario es vinculado y en ese momento tienes la oportunidad de realizar algunas acciones usando el objeto ``$tarea`` (por ejemplo, persistirlo a la base de datos) antes de redirigir al usuario a otra página (por ejemplo, una página de "agradecimiento" o "éxito").

.. note::

   Redirigir a un usuario después de un exitoso envío de formularios evita que el usuario pueda hacer clic en "actualizar" y volver a enviar los datos.

.. index::
   single: Formularios; Validación

Validando formularios
---------------------

En la sección anterior, aprendiste cómo se puede presentar un formulario con datos válidos o no válidos. En Symfony2, la validación se aplica al objeto subyacente (por ejemplo, ``Tarea``). En otras palabras, la cuestión no es si el "formulario" es válido, sino más bien si el objeto ``$tarea`` es válido después de aplicarle los datos enviados en el formulario. Invocar a ``$formulario->isValid()`` es un atajo que pregunta al objeto ``$tarea`` si tiene datos válidos o no.

La validación se realiza añadiendo un conjunto de reglas (llamadas restricciones) a una clase. Para ver esto en acción, añade restricciones de validación para que el campo ``tarea`` no pueda estar vacío y el campo ``fechaVencimiento`` no pueda estar vacío y debe ser un número no negativo:

.. configuration-block::

    .. code-block:: yaml

        # Acme/TareaBundle/Resources/config/validation.yml
        Acme\TareaBundle\Entity\Tarea:
            properties:
                tarea:
                    - NotBlank: ~
                fechaVencimiento:
                    - NotBlank: ~
                    - Type: \DateTime

    .. code-block:: xml

        <!-- Acme/TareaBundle/Resources/config/validation.xml -->
        <class name="Acme\TareaBundle\Entity\Tarea">
            <property name="tarea">
                <constraint name="NotBlank" />
            </property>
            <property name="fechaVencimiento">
                <constraint name="NotBlank" />
                <constraint name="Type">
                    <value>\DateTime</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // Acme/TareaBundle/Entity/Tarea.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Tarea
        {
            /**
             * @Assert\NotBlank()
             */
            public $tarea;

            /**
             * @Assert\NotBlank()
             * @Assert\Type("\DateTime")
             */
            protected $fechaVencimiento;
        }

    .. code-block:: php

        // Acme/TareaBundle/Entity/Tarea.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        class Tarea
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('tarea', new NotBlank());

                $metadatos->addPropertyConstraint('fechaVencimiento', new NotBlank());
                $metadatos->addPropertyConstraint('fechaVencimiento', new Type('\DateTime'));
            }
        }

¡Eso es todo! Si vuelves a enviar el formulario con datos no válidos, verás replicados los errores correspondientes en el formulario.

.. _book-forms-html5-validation-disable:

.. sidebar:: Validación HTML5

   A partir de HTML5, muchos navegadores nativamente pueden imponer ciertas restricciones de validación en el lado del cliente. La validación más común se activa al reproducir un atributo ``required`` en los campos que son obligatorios. Para el navegador compatible con HTML5, esto se traducirá en un mensaje nativo del navegador que muestra si el usuario intenta enviar el formulario con ese campo en blanco.
   
   Los formularios generados sacan el máximo provecho de esta nueva característica añadiendo atributos HTML razonables que desencadenan la validación. La validación del lado del cliente, sin embargo, se puede desactivar añadiendo el atributo ``novalidate`` de la etiqueta ``form`` o ``formnovalidate`` a la etiqueta de envío. Esto es especialmente útil cuando deseas probar tus limitaciones en el lado del la validación del servidor, pero su navegador las previene, por ejemplo, la presentación de campos en blanco.

La validación es una característica muy poderosa de Symfony2 y tiene su propio :doc:`capítulo dedicado </book/validation>`.

.. index::
   single: Formularios; Validación de grupos

.. _book-forms-validation-groups:

Validando grupos
~~~~~~~~~~~~~~~~

.. tip::

    Si no estás utilizando la :ref:`validación de grupos <book-validation-validation-groups>`, entonces puedes saltarte esta sección.

Si tu objeto aprovecha la :ref:`validación de grupos <book-validation-validation-groups>`, tendrás que especificar la validación de grupos que utiliza tu formulario::

    $form = $this->createFormBuilder($usuarios, array(
        'validation_groups' => array('registro'),
    ))->add(...)
    ;

Si vas a crear :ref:`clases form <book-form-creating-form-classes>` (una buena práctica), entonces tendrás que agregar lo siguiente al método ``getDefaultOptions()``::

    public function getDefaultOptions(array $opciones)
    {
        return array(
            'validation_groups' => array('registro')
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

También puedes crear tus propios tipos de campo personalizados. Este tema se trata en el artículo ":doc:`/cookbook/form/create_custom_field_type`" del recetario.

.. index::
   single: Formularios; Opciones de tipo de campo

Opciones del tipo de campo
~~~~~~~~~~~~~~~~~~~~~~~~~~

Cada tipo de campo tiene una serie de opciones que puedes utilizar para configurarlo.
Por ejemplo, el campo ``fechaVencimiento`` se está traduciendo como 3 cajas de selección. Sin embargo, puedes configurar el :doc:`campo de fecha </reference/forms/types/date>` para que sea interpretado como un cuadro de texto (donde el usuario introduce la fecha como una cadena en el cuadro)::

    ->add('fechaVencimiento', 'date', array('widget' => 'single_text'))

.. image:: /images/book/form-simple2.png
    :align: center

Cada tipo de campo tiene una diferente serie de opciones que le puedes pasar.
Muchas de ellas son específicas para el tipo de campo y puedes encontrar los detalles en la documentación de cada tipo.

.. sidebar:: La opción ``required``

    La opción más común es la opción ``required``, la cual puedes aplicar a cualquier campo. De manera predeterminada, la opción ``required`` está establecida en ``true``, lo cual significa que los navegadores preparados para HTML5 aplicarán la validación en el cliente si el campo se deja en blanco. Si no deseas este comportamiento, establece la opción ``required`` en tu campo a ``false`` o :ref:`desactiva la validación de HTML5 <book-forms-html5-validation-disable>`.
    
    También ten en cuenta que al establecer la opción ``required`` a ``true`` **no** resultará en aplicar la validación de lado del servidor. En otras palabras, si un usuario envía un valor en blanco para el campo (ya sea con un navegador antiguo o un servicio web, por ejemplo), será aceptado como un valor válido a menos que utilices la validación de restricción ``NotBlank`` o ``NotNull`` de Symfony.
    
    En otras palabras, la opción ``required`` es "agradable", pero ciertamente *siempre* se debe utilizar de lado del servidor.

.. index::
   single: Formularios; Adivinando el tipo de campo

.. _book-forms-field-guessing:

Adivinando el tipo de campo
---------------------------

Ahora que haz añadido metadatos de validación a la clase ``Tarea``, Symfony ya sabe un poco sobre tus campos. Si le permites, Symfony puede "adivinar" el tipo de tu campo y configurarlo por ti. En este ejemplo, Symfony puede adivinar a partir de las reglas de validación de ambos campos, ``tarea`` es un campo de texto normal y ``fechaVencimiento`` es un campo ``date``::

    public function nuevaAction()
    {
        $tarea = new Tarea();

        $formulario = $this->createFormBuilder($tarea)
            ->add('tarea')
            ->add('fechaVencimiento', null, array('widget' => 'single_text'))
            ->getForm();
    }

El "adivino" se activa cuando omites el segundo argumento del método ``add()`` (o si le pasas  ``null``). Si pasas una matriz de opciones como tercer argumento (hecho por ``fechaVencimiento`` arriba), estas opciones se aplican al campo adivinado.

.. caution::

    Si tu formulario utiliza una validación de grupo específica, el adivinador del tipo de campo seguirá considerando *todas* las restricciones de validación cuando adivina el tipo de campo (incluyendo las restricciones que no son parte de la validación de grupo utilizada).

.. index::
   single: Formularios; Adivinando el tipo de campo

Opciones para adivinar el tipo de campo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Además de adivinar el "tipo" de un campo, Symfony también puede tratar de adivinar los valores correctos de una serie de opciones de campo:

* ``required``: La opción ``required`` se puede adivinar basándose en las reglas de validación (es decir, el campo es ``NotBlank`` o ``NotNull``) o los metadatos de Doctrine (es decir, el campo puede ser nulo - ``nullable``). Esto es muy útil, ya que tu validación de lado del cliente se ajustará automáticamente a tus reglas de validación.

* ``min_length``: Si el campo es una especie de campo de texto, entonces la opción ``min_length`` se puede adivinar a partir de las restricciones de validación (si se utiliza ``minLength`` o ``Min``) o de los metadatos de Doctrine (vía la longitud del campo).

* ``max_length``: Similar a ``min_length``, la longitud máxima también se puede adivinar.

.. note::

  Estas opciones de campo *sólo* se adivinan si estás utilizando Symfony para averiguar el tipo de campo (es decir, omitir o pasar ``null`` como segundo argumento de ``add()``).

Si quieres cambiar uno de los valores adivinados, lo puedes redefinir pasando la opción en la matriz de las opciones del campo::

    ->add('tarea', null, array('min_length' => 4))

.. index::
   single: Formularios; Reproduciendo en una plantilla

.. _form-rendering-template:

Reproduciendo un formulario en una plantilla
--------------------------------------------

Hasta ahora, haz visto cómo se puede reproducir todo el formulario con una sola línea de código. Por supuesto, generalmente necesitarás mucha más flexibilidad al reproducirlo:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TareaBundle/Resources/views/Default/nueva.html.twig #}

        <form action="{{ path('tarea_nyeva') }}" method="post" {{ form_enctype(form) }}>
            {{ form_errors(form) }}

            {{ form_row(form.tarea) }}
            {{ form_row(form.fechaVencimiento) }}

            {{ form_rest(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <!-- // src/Acme/TareaBundle/Resources/views/Default/nuevaAction.html.php -->

        <form action="<?php echo $view['router']->generate('tarea_nueva') ?>" method="post" <?php echo $view['form']->enctype($form) ?>>
            <?php echo $view['form']->errors($form) ?>

            <?php echo $view['form']->row($form['tarea']) ?>
            <?php echo $view['form']->row($form['fechaVencimiento']) ?>

            <?php echo $view['form']->rest($formulario) ?>

            <input type="submit" />
        </form>

Echemos un vistazo a cada parte:

* ``form_enctype(form)`` - Si por lo menos un campo es un campo de carga de archivos, se reproduce el obligado ``enctype="multipart/form-data"``;

* ``form_errors(form)`` - Reproduce cualquier error global para todo el formulario (los errores específicos de campo se muestran junto a cada campo);

* ``form_row(form.dueDate)`` - Reproduce la etiqueta, cualquier error, y el elemento gráfico HTML del formulario para el campo en cuestión (por ejemplo, ``fechaVencimiento``), por omisión, en el interior un elemento ``div``;

* ``form_rest(form)`` - Reproduce todos los campos que aún no se han representado.
  Por lo general es buena idea realizar una llamada a este ayudante en la parte inferior de cada formulario (en caso de haber olvidado sacar un campo o si no quieres preocuparte de reproducir manualmente los campos ocultos). Este ayudante también es útil para tomar ventaja de la :ref:`Protección CSRF <forms-csrf>` automática.

La mayor parte del trabajo la realiza el ayudante ``form_row``, el cual de manera predeterminada reproduce la etiqueta, los errores y el elemento gráfico HTML de cada campo del formulario dentro de una etiqueta ``div``. En la sección :ref:`form-theming`, aprenderás cómo puedes personalizar ``form_row`` en muchos niveles diferentes.

.. index::
   single: Formularios; Reproduciendo a mano cada campo

Reproduciendo cada campo a mano
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

El ayudante ``form_row`` es magnífico porque rápidamente puedes reproducir cada campo del formulario (y también puedes personalizar el formato utilizado para la "fila"). Pero, puesto que la vida no siempre es tan simple, también puedes reproducir cada campo totalmente a mano. El producto final del siguiente fragmento es el mismo que cuando usas el ayudante ``form_row``:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_errors(form) }}

        <div>
            {{ form_label(form.tarea) }}
            {{ form_errors(form.tarea) }}
            {{ form_widget(form.tarea) }}
        </div>

        <div>
            {{ form_label(form.fechaVencimiento) }}
            {{ form_errors(form.fechaVencimiento) }}
            {{ form_widget(form.fechaVencimiento) }}
        </div>

        {{ form_rest(form) }}

    .. code-block:: html+php

        <?php echo $view['form']->errors($formulario) ?>

        <div>
            <?php echo $view['form']->label($form['tarea']) ?>
            <?php echo $view['form']->errors($form['tarea']) ?>
            <?php echo $view['form']->widget($form['tarea']) ?>
        </div>

        <div>
            <?php echo $view['form']->label($form['fechaVencimiento']) ?>
            <?php echo $view['form']->errors($form['fechaVencimiento']) ?>
            <?php echo $view['form']->widget($form['fechaVencimiento']) ?>
        </div>

        <?php echo $view['form']->rest($formulario) ?>

Si la etiqueta generada automáticamente para un campo no es del todo correcta, la puedes especificar explícitamente:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.tarea, 'Descripción de tarea') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['tarea'], 'Descripción de tarea') ?>

Por último, algunos tipos de campo tienen más opciones para su representación que puedes pasar al elemento gráfico. Estas opciones están documentadas con cada tipo, pero una de las opciones común es ``attr``, la cual te permite modificar los atributos en el elemento del formulario.
El siguiente debería añadir la clase ``campo_tarea`` al campo de entrada de texto reproducido:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.tarea, { 'attr': {'class': 'campo_tarea'} }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['tarea'], array(
            'attr' => array('class' => 'campo_tarea'),
        )) ?>

Referencia de funciones de plantilla Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si estás utilizando Twig, hay disponible una referencia completa de las funciones de reproducción de formularios en el :doc:`Manual de referencia </reference/forms/twig_reference>`.
Lee esto para conocer todo acerca de los ayudantes y opciones disponibles que puedes utilizar con cada uno.

.. index::
   single: Formularios; Creando clases ``Form``

.. _book-form-creating-form-classes:

Creando clases ``Form``
-----------------------

Como hemos visto, puedes crear un formulario y utilizarlo directamente en un controlador.
Sin embargo, una mejor práctica es construir el formulario en una clase separada, independiente de las clases PHP, la cual puedes reutilizar en cualquier lugar de tu aplicación. Crea una nueva clase que albergará la lógica para la construcción del formulario de la tarea:

.. code-block:: php

    // src/Acme/TareaBundle/Form/Type/TareaType.php

    namespace Acme\TareaBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class TareaType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $opciones)
        {
            $builder->add('tarea');
            $builder->add('fechaVencimiento', null, array('widget' => 'single_text'));
        }

        public function getName()
        {
            return 'tarea';
        }
    }

Esta nueva clase contiene todas las indicaciones necesarias para crear el formulario de la tarea (observa que el método ``getName()`` debe devolver un identificador único para este "tipo" de formulario). La puedes utilizar para construir rápidamente un objeto formulario en el controlador:

.. code-block:: php

    // src/Acme/TareaBundle/Controller/DefaultController.php

    // agrega esta nueva declaración use en lo alto de la clase
    use Acme\TareaBundle\Form\Type\TareaType;

    public function nuevaAction()
    {
        $tarea = // ...
        $form = $this->createForm(new TareaType(), $tarea);

        // ...
    }

Colocar la lógica del formulario en su propia clase significa que fácilmente puedes reutilizar el formulario en otra parte del proyecto. Esta es la mejor manera de crear formularios, pero la decisión en última instancia, depende de ti.

.. sidebar:: Configurando ``data_class``

    Cada formulario tiene que conocer el nombre de la clase que contiene los datos subyacentes (por ejemplo, ``Acme\TareaBundle\Entity\Tarea``). Por lo general, esto sólo se adivina basándose en el objeto pasado como segundo argumento de ``createForm`` (es decir, ``$tarea``). Más tarde, cuando comiences a incorporar formularios, esto ya no será suficiente. Así que, si bien no siempre es necesario, generalmente es buena idea especificar explícitamente la opción ``data_class`` añadiendo lo siguiente al tipo de tu clase formulario::

        public function getDefaultOptions(array $opciones)
        {
            return array(
                'data_class' => 'Acme\TareaBundle\Entity\Tarea',
            );
        }

.. index::
   pair: Formularios; Doctrine

Formularios y Doctrine
----------------------

El objetivo de un formulario es traducir los datos de un objeto (por ejemplo, ``Tarea``) a un formulario HTML y luego traducir los datos enviados por el usuario al objeto original. Como tal, el tema de la persistencia del objeto ``Tarea`` a la base de datos es del todo ajeno al tema de los formularios. Pero, si haz configurado la clase ``Tarea`` para persistirla a través de Doctrine (es decir, que le haz añadido :ref:`metadatos de asignación <book-doctrine-adding-mapping>`), entonces persistirla después de la presentación de un formulario se puede hacer cuando el formulario es válido::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($tarea);
        $em->flush();

        return $this->redirect($this->generateUrl('tarea_exito'));
    }

Si por alguna razón, no tienes acceso a tu objeto ``$tarea`` original, lo puedes recuperar desde el formulario::

    $tarea = $formulario->getData();

Para más información, consulta el capítulo :doc:`ORM de Doctrine </book/doctrine>`.

La clave es entender que cuando el formulario está vinculado, los datos presentados inmediatamente se transfieren al objeto subyacente. Si deseas conservar los datos, sólo tendrás que conservar el objeto en sí (el cual ya contiene los datos presentados).

.. index::
   single: Formularios; Integrando formularios

Integrando formularios
----------------------

A menudo, querrás crear un formulario que incluye campos de muchos objetos diferentes. Por ejemplo, un formulario de registro puede contener datos que pertenecen a un objeto ``Usuario``, así como a muchos objetos ``Domicilio``. Afortunadamente, esto es fácil y natural con el componente ``Form``.

Integrando un solo objeto
~~~~~~~~~~~~~~~~~~~~~~~~~

Supongamos que cada ``Tarea`` pertenece a un simple objeto ``Categoría``. Inicia, por supuesto, creando el objeto ``Categoría``::

    // src/Acme/TareaBundle/Entity/Categoria.php
    namespace Acme\TareaBundle\Entity;

    use Symfony\Component\Validator\Constraints as Assert;

    class Categoria
    {
        /**
         * @Assert\NotBlank()
         */
        public $nombre;
    }

A continuación, añade una nueva propiedad ``categoría`` a la clase ``Tarea``::

    // ...

    class Tarea
    {
        // ...

        /**
         * @Assert\Type(type="Acme\TareaBundle\Entity\Categoria")
         */
        protected $categoria;

        // ...

        public function getCategoria()
        {
            return $this->categoria;
        }

        public function setCategoria(Categoria $categoria = null)
        {
            $this->categoria = $categoria;
        }
    }

Ahora que hemos actualizado tu aplicación para reflejar las nuevas necesidades, crea una clase formulario para que el usuario pueda modificar un objeto ``Categoría``::

    // src/Acme/TareaBundle/Form/Type/CategoriaType.php
    namespace Acme\TareaBundle\Form\Type;

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
                'data_class' => 'Acme\TareaBundle\Entity\Categoria',
            );
        }

        public function getNombre()
        {
            return 'categoria';
        }
    }

El objetivo final es permitir que la ``Categoría`` de una ``Tarea`` sea modificada justo dentro del mismo formulario de la tarea. Para lograr esto, añade un campo ``categoría`` al objeto ``TareaType`` cuyo tipo es una instancia de la nueva clase ``CategoriaType``:

.. code-block:: php

    public function buildForm(FormBuilder $builder, array $opciones)
    {
        // ...

        $builder->add('categoria', new CategoriaType());
    }

Los campos de ``CategoriaType`` ahora se pueden reproducir junto a los de la clase ``TareaType``. Reproduce los campos de la ``Categoría`` de la misma manera que los campos de la ``Tarea`` original:

.. configuration-block::

    .. code-block:: html+jinja

        {# ... #}

        <h3>Categoría</h3>
        <div class="categoria">
            {{ form_row(form.categoria.nombre) }}
        </div>

        {{ form_rest(form) }}
        {# ... #}

    .. code-block:: html+php

        <!-- ... -->

        <h3>Categoría</h3>
        <div class="categoria">
            <?php echo $view['form']->row($formulario['categoria']['nombre']) ?>
        </div>

        <?php echo $view['form']->rest($formulario) ?>
        <!-- ... -->

Cuando el usuario envía el formulario, los datos presentados para los campos de la ``Categoría`` se utilizan para construir una instancia de la ``Categoría``, que se establece en el campo ``Categoría`` de la instancia ``Tarea``.

La instancia de ``Categoría`` es accesible naturalmente vía ``$tarea->getCategoria()`` y la puedes persistir en la base de datos o utilizarla como necesites.

Integrando una colección de formularios
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

También puedes integrar una colección de formularios en un formulario. Esto se consigue usando el tipo de campo ``collection``. Para más información, consulta :doc:`referencia del tipo de campo </reference/forms/types/collection>`.

.. index::
   single: Formularios; Tematizando
   single: Formularios; Campos personalizados

.. _form-theming:

Tematizando formularios
-----------------------

Puedes personalizar cómo se reproduce cada parte de un formulario. Eres libre de cambiar la forma en que se reproduce cada "fila" del formulario, cambiar el formato que sirve para reproducir errores, e incluso personalizar la forma en que se debe reproducir una etiqueta ``textarea``. Nada está fuera de límites, y puedes usar diferentes personalizaciones en diferentes lugares.

Symfony utiliza plantillas para reproducir todas y cada una de las partes de un formulario, como las etiquetas ``label``, etiquetas ``input``, mensajes de error y todo lo demás.

En Twig, cada "fragmento" del formulario está representado por un bloque Twig. Para personalizar alguna parte de cómo se reproduce un formulario, sólo hay que reemplazar el bloque adecuado.

En PHP, cada "fragmento" del formulario se reproduce vía un archivo de plantilla individual.
Para personalizar cualquier parte de cómo se reproduce un formulario, sólo hay que reemplazar la plantilla existente creando una nueva.

Para entender cómo funciona esto, vamos a personalizar el fragmento ``form_row`` añadiendo un atributo de clase al elemento ``div`` que rodea cada fila. Para ello, crea un nuevo archivo de plantilla que almacenará el nuevo marcado:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TareaBundle/Resources/views/Form/fields.html.twig #}

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

        <!-- src/Acme/TareaBundle/Resources/views/Form/field_row.html.php -->

        <div class="form_row">
            <?php echo $view['form']->label($formulario, $label) ?>
            <?php echo $view['form']->errors($formulario) ?>
            <?php echo $view['form']->widget($formulario, $parameters) ?>
        </div>

El fragmento ``field_row`` del formulario se usa cuando se reproduce la mayoría de los campos a través de la función ``form_row``. Para decir al componente ``Form`` que utilice tu nuevo fragmento ``field_row`` definido anteriormente, añade lo siguiente en la parte superior de la plantilla que reproduce el formulario:

.. configuration-block:: php

    .. code-block:: html+jinja

        {# src/Acme/TareaBundle/Resources/views/Default/nueva.html.twig #}

        {% form_theme form 'AcmeTareaBundle:Form:fields.html.twig' %}

        <form ...>

    .. code-block:: html+php

        <!-- src/Acme/TareaBundle/Resources/views/Default/nueva.html.php -->

        <?php $view['form']->setTheme($form, array('AcmeTareaBundle:Form')) ?>

        <form ...>

La etiqueta ``form_theme`` (en Twig) "importa" los fragmentos definidos en la plantilla dada y los utiliza al reproducir el formulario. `En otras palabras, cuando se llame a la función ``form_row`` más adelante en esta plantilla, se utilizará el bloque ``field_row`` de tu tema personalizado (en lugar del bloque  ``field_row`` predeterminado suministrado con Symfony).

Para personalizar cualquier parte de un formulario, sólo tienes que reemplazar el fragmento apropiado. Saber exactamente qué bloque sustituir es el tema de la siguiente sección.

Para una explicación más extensa, consulta :doc:`/cookbook/form/form_customization`.

.. index::
   single: Formularios; nombrando fragmentos de plantilla

.. _form-template-blocks:

Nombrando fragmentos de formulario
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En Symfony, cada parte que se reproduce de un formulario - elementos HTML de formulario, errores, etiquetas, etc. - se define en un tema base, el cual es una colección de bloques en Twig y una colección de archivos de plantilla en PHP.

En Twig, cada bloque necesario se define en un solo archivo de plantilla (`form_div_layout.html.twig`_) que vive dentro de `Twig Bundle`_. Dentro de este archivo, puedes ver todos los bloques necesarios para reproducir un formulario y cada tipo de campo predeterminado.

En PHP, los fragmentos son archivos de plantilla individuales. De manera predeterminada se encuentran en el directorio ``Resources/views/Form`` del paquete de la plataforma (`ver en GitHub`_).

El nombre de cada fragmento sigue el mismo patrón básico y se divide en dos partes, separadas por un solo carácter de subrayado (``_``). Algunos ejemplos son:

* ``field_row`` - usado por ``form_row`` para reproducir la mayoría de los campos;
* ``textarea_widget`` - usado por ``form_widget`` para reproducir un campo de tipo ``textarea``;
* ``field_errors`` - usado por ``form_errors`` para reproducir los errores de un campo;

Cada fragmento sigue el mismo patrón básico: ``tipo_parte``. La porción ``tipo`` corresponde al *tipo* del campo que se está reproduciendo (por ejemplo, ``textarea``, ``checkbox``, ``date``, etc.), mientras que la porción ``parte`` corresponde a *qué* se está reproduciendo (por ejemplo, ``label``, ``widget``, ``errores``, etc.). De manera predeterminada, hay cuatro posibles *partes* de un formulario que puedes reproducir:

+-------------+----------------------------+----------------------------------------------------------------------+
| ``label``   | (p. ej. ``field_label``)   | reproduce la etiqueta del campo                                      |
+-------------+----------------------------+----------------------------------------------------------------------+
| ``widget``  | (p. ej. ``field_widget``)  | reproduce el HTML del campo                                          |
+-------------+----------------------------+----------------------------------------------------------------------+
| ``errors``  | (p. ej. ``field_errors``)  | reproduce los errores del campo                                      |
+-------------+----------------------------+----------------------------------------------------------------------+
| ``row``     | (p. ej. ``field_row``)     | reproduce el renglón completo (etiqueta, elemento gráfico y errores) |
+-------------+----------------------------+----------------------------------------------------------------------+

.. note::

    En realidad, hay otras 3 *partes* - ``filas``, ``resto`` y ``enctype`` - pero que rara vez o nunca te tienes que preocupar de cómo reemplazarlas.

Al conocer el tipo de campo (por ejemplo, ``textarea``) y cual parte deseas personalizar (por ejemplo, ``widget``), puedes construir el nombre del fragmento que se debe redefinir (por ejemplo, ``textarea_widget``).

.. index::
   single: Formularios; Heredando fragmentos de plantilla

Heredando fragmentos de plantilla
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En algunos casos, parece que falta el fragmento que deseas personalizar.
Por ejemplo, no hay fragmento ``textarea_errors`` en los temas predeterminados provistos con Symfony. Entonces, ¿cómo se reproducen los errores de un campo ``textarea``?

La respuesta es: a través del fragmento ``field_errors``. Cuando Symfony reproduce los errores del tipo ``textarea``, primero busca un fragmento ``textarea_errors`` antes de caer de nuevo al fragmento ``field_errors``. Cada tipo de campo tiene un tipo *padre* (el tipo primario del ``textarea`` es ``field``), y Symfony utiliza el fragmento para el tipo del padre si no existe el fragmento base.

Por lo tanto, para sustituir *sólo* los errores de los campos ``textarea``, copia el fragmento ``field_errors``, cámbiale el nombre a ``textarea_errors`` y personalizarlo. Para sustituir la reproducción predeterminada para error de *todos* los campos, copia y personaliza el fragmento ``field_errors`` directamente.

.. tip::

    El tipo "padre" de cada tipo de campo está disponible en la :doc:`referencia del tipo form </reference/forms/types>` para cada tipo de campo.

.. index::
   single: Formularios; Tematizando globalmente

Tematizando formularios globalmente
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En el ejemplo anterior, utilizaste el ayudante ``form_theme`` (en Twig) para "importar" fragmentos de formulario personalizados *sólo* para ese formulario. También puedes decirle a Symfony que importe formularios personalizados a través de tu proyecto.

Twig
....

Para incluir automáticamente en *todas* las plantillas los bloques personalizados de la plantilla ``fields.html.twig`` creada anteriormente, modifica el archivo de configuración de tu aplicación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeTareaBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <twig:config ...>
                <twig:form>
                    <resource>AcmeTaskBundle:Form:fields.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php

        $container->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'AcmeTareaBundle:Form:fields.html.twig',
             ))
            // ...
        ));

Ahora se utilizan todos los bloques dentro de la plantilla ``fields.html.twig`` a nivel global para definir el formulario producido.

.. sidebar::  Personalizando toda la salida del formulario en un único archivo con Twig

    En la Twig, también puedes personalizar el bloque correcto de un formulario dentro de la plantilla donde se necesita esa personalización:

    .. code-block:: html+jinja

        {% extends '::base.html.twig' %}

        {# import "_self" como el tema del formulario #}
        {% form_theme form _self %}

        {# hace la personalización del fragmento del formulario #}
        {% block field_row %}
            {# produce la fila del campo personalizado #}
        {% endblock field_row %}

        {% block contenido %}
            {# ... #}

            {{ form_row(form.tarea) }}
        {% endblock %}

    La etiqueta ``{% form_theme form_self %}`` permite personalizar bloques directamente dentro de la plantilla que utilizará las personalizaciones. Utiliza este método para crear rápidamente formularios personalizados que sólo son necesarios en una sola plantilla.

PHP
...

Para incluir automáticamente en *todas* las plantillas personalizadas del directorio `Acme/TareaBundle/Resources/views/Form` creado anteriormente, modifica el archivo de configuración de tu aplicación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        framework:
            templating:
                form:
                    resources:
                        - 'AcmeTareaBundle:Form'
        # ...


    .. code-block:: xml

        <!-- app/config/config.xml -->

        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>AcmeTareaBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php

        $contenedor->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'AcmeTareaBundle:Form',
             )))
            // ...
        ));

Cualquier fragmento dentro del directorio `Acme/TareaBundle/Resources/views/Form` ahora se utiliza globalmente para definir la salida del formulario.

.. index::
   single: Formularios; Protección CSRF

.. _forms-csrf:

Protección CSRF
---------------

CSRF (Cross-site request forgery) - o `Falsificación de petición en sitios cruzados`_  - es un método por el cual un usuario malintencionado intenta hacer que tus usuarios legítimos, sin saberlo, presenten datos que no tienen la intención de enviar. Afortunadamente, los ataques CSRF se pueden prevenir usando un elemento CSRF dentro de tus formularios.

La buena nueva es que, por omisión, Symfony integra y valida elementos CSRF automáticamente. Esto significa que puedes aprovechar la protección CSRF sin hacer nada. De hecho, ¡cada formulario en este capítulo se ha aprovechado de la protección CSRF!

La protección CSRF funciona añadiendo un campo oculto al formulario - por omisión denominado ``_token`` - el cual contiene un valor que sólo tú y tu usuario conocen. Esto garantiza que el usuario - y no alguna otra entidad - es el que presenta dichos datos.
Symfony automáticamente valida la presencia y exactitud de este elemento.

El campo ``_token`` es un campo oculto y será reproducido automáticamente si se incluye la función ``form_rest()`` de la plantilla, la cual garantiza que se presenten todos los campos producidos.

El elemento CSRF se puede personalizar formulario por formulario. Por ejemplo::

    class TareaType extends AbstractType
    {
        // ...

        public function getDefaultOptions(array $opciones)
        {
            return array(
                'data_class'      => 'Acme\TareaBundle\Entity\Tarea',
                'csrf_protection' => true,
                'csrf_field_name' => '_token',
                // una clave única para ayudar a generar el elemento secreto
                'intention'       => 'task_item',
            );
        }

        // ...
    }

Para desactivar la protección CSRF, fija la opción ``csrf_protection`` a false.
Las personalizaciones también se pueden hacer a nivel global en tu proyecto. Para más información, consulta la sección :ref:`referencia de configuración de formularios </reference-frameworkbundle-forms>`.

.. note::

    La opción ``intention`` es opcional pero mejora considerablemente la seguridad del elemento generado produciendo uno diferente para cada formulario.

Consideraciones finales
-----------------------

Ahora ya conoces todos los bloques de construcción necesarios para construir formularios complejos y funcionales para tu aplicación. Cuando construyas formularios, ten en cuenta que el primer objetivo de un formulario es traducir los datos de un objeto (``Tarea``) a un formulario HTML para que el usuario pueda modificar esos datos. El segundo objetivo de un formulario es tomar los datos presentados por el usuario y volverlos a aplicar al objeto.

Todavía hay mucho más que aprender sobre el poderoso mundo de los formularios, tal como la forma de :doc:`manejar con Doctrine los archivos subidos </cookbook/doctrine/file_uploads>` o cómo crear un formulario donde puedes agregar una serie dinámica de subformularios (por ejemplo, una lista de tareas donde puedes seguir añadiendo más campos a través de Javascript antes de presentarlo). Consulta el recetario para estos temas. Además, asegúrate de apoyarte en  la documentación :doc:`referencia de tipo de campo </reference/forms/types>`, que incluye ejemplos de cómo utilizar cada tipo de campo y sus opciones.

Aprende más en el recetario
---------------------------

* :doc:`/cookbook/doctrine/file_uploads`
* :doc:`Referencia del campo file </reference/forms/types/file>`
* :doc:`Creando tipos de campo personalizados </cookbook/form/create_custom_field_type>`
* :doc:`/cookbook/form/form_customization`

.. _`Componente Form de Symfony2`: https://github.com/symfony/Form
.. _`DateTime`: http://php.net/manual/es/class.datetime.php
.. _`Twig Bridge`: https://github.com/symfony/symfony/tree/master/src/Symfony/Bridge/Twig
.. _`form_div_base.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_base.html.twig
.. _`Falsificación de petición en sitios cruzados`: http://es.wikipedia.org/wiki/Cross_Site_Request_Forgery _`ver en GitHub`:   https://github.com/symfony/symfony/tree/master/src/Symfony/Bundle/FrameworkBundle/Resources/views/Form
