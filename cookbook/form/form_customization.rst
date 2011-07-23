Cómo personalizar la reproducción de un formulario
==================================================

Symfony ofrece una amplia variedad de formas para personalizar cómo se reproduce un formulario.
En esta guía, aprenderás cómo personalizar cada parte posible de tu formulario con el menor esfuerzo posible si utilizas Twig o PHP como tu motor de plantillas.

Fundamentos de la reproducción de formularios
---------------------------------------------

Recuerda que ``label``, error y los elementos gráficos HTML de un campo de formulario se pueden reproducir fácilmente usando la función ``form_row`` de Twig o el método ayudante ``row`` de PHP:

.. code-block:: jinja

    {{ form_row(form.edad) }}

.. code-block:: php

    <?php echo $view['form']->row($formulario['edad']) }} ?>

También puedes reproducir cada una de las tres partes del campo individualmente:

.. code-block:: jinja

    <div>
        {{ form_label(form.edad) }}
        {{ form_errors(form.edad) }}
        {{ form_widget(form.edad) }}
    </div>

.. code-block:: php

    <div>
        <?php echo $view['form']->label($formulario['edad']) }} ?>
        <?php echo $view['form']->errors($formulario['edad']) }} ?>
        <?php echo $view['form']->widget($formulario['edad']) }} ?>
    </div>

En ambos casos, la etiqueta, errores y elementos gráficos del formulario HTML se reproducen con un conjunto de marcas que se incluyen de serie con Symfony. Por ejemplo, ambas plantillas anteriores reproducirán:

.. code-block:: html

    <div>
        <label for="form_edad">Edad</label>
        <ul>
            <li>Este campo es obligatorio</li>
        </ul>
        <input type="number" id="form_edad" name="form[edad]" />
    </div>

para crear prototipos rápidamente y probar un formulario, puedes reproducir el formulario completo con una sola línea:

.. code-block:: jinja

    {{ form_widget(form) }}

.. code-block:: php

    <?php echo $view['form']->widget($formulario) }} ?>

El resto de esta receta debe explicar cómo se puede modificar cada parte del marcado del formulario en varios niveles diferentes. Para más información sobre la forma de reproducción en general, consulta :ref:`form-rendering-template`.

¿Qué son los temas de formulario?
---------------------------------

Symfony utiliza fragmentos de formulario - una parte de una plantilla que sólo reproduce una pequeña parte de un formulario - para reproducir todas las partes de un formulario - - etiquetas de campo, errores, campos de texto ``input``, etiquetas ``select``, etc.

Los fragmentos se definen como bloques en Twig y como archivos de plantilla en PHP.

A *tema* no es más que un conjunto de fragmentos que deseas utilizar al reproducir un formulario. En otras palabras, si deseas personalizar una parte de cómo reproducir un formulario, importa el *tema* que contiene una personalización apropiada de los fragmentos del formulario.

Symfony viene con un tema predeterminado (`form_div_base.html.twig`_ en Twig y ``FrameworkBundle:Form`` en PHP) que define todos y cada uno de los fragmentos necesarios para reproducir todas las partes de un formulario.

En la siguiente sección aprenderás cómo personalizar un tema redefiniendo todos o algunos de sus fragmentos.

Por ejemplo, cuando reproduces el elemento gráfico de un campo de tipo ``entero``, se genera un campo ``input`` como ``número``.

.. code-block:: html+jinja

    {{ form_widget(form.edad) }}

.. code-block:: php

    <?php echo $view['form']->widget($formulario['edad']) ?>

reproduce:

.. code-block:: html

    <input type="number" id="form_edad" name="form[edad]" required="required" value="33" />

Internamente, Symfony utiliza el fragmento ``integer_widget`` para reproducir el campo.
Esto se debe a que el tipo de campo es ``entero`` y estás reproduciendo su ``elemento gráfico`` (en comparación a ``label`` o ``errores``).

En Twig de manera predeterminada en el bloque ``integer_widget`` de la plantilla `form_div_base.html.twig`_.

En PHP sería más bien el archivo ``integer_widget.html.php`` ubicado en el directorio ``FrameworkBundle/Resources/views/Form``.

La implementación predeterminada del fragmento ``integer_widget`` tiene el siguiente aspecto:

.. code-block:: jinja

    {% block integer_widget %}
        {% set type = type|default('number') %}
        {{ block('field_widget') }}
    {% endblock integer_widget %}

.. code-block:: html+php

    <!-- integer_widget.html.php -->

    <?php echo $view['form']->renderBlock('field_widget', array('type' => isset($type) ? $type : "number")) ?>

Como puedes ver, este fragmento reproduce otro fragmento - ``field_widget``:

.. code-block:: html+jinja

    {% block field_widget %}
        {% set type = type|default('text') %}
        <input type="{{ type }}" {{ block('widget_attributes') }} value="{{ value }}" />
    {% endblock field_widget %}

.. code-block:: html+php

    <!-- FrameworkBundle/Resources/views/Form/field_widget.html.php -->

    <input
        type="<?php echo isset($type) ? $view->escape($type) : "text" ?>"
        value="<?php echo $view->escape($value) ?>"
        <?php echo $view['form']->renderBlock('attributes') ?>
    />

El punto es que los fragmentos dictan la salida HTML de cada parte de un formulario. Para personalizar la salida del formulario, sólo tienes que identificar y redefinir el fragmento correcto. Un conjunto de estos fragmentos personalizados del formulario se conoce como un "tema" de formulario.
Al reproducir un formulario, puedes elegir el/los tema(s) que deseas aplicar al formulario.

En Twig un tema es un sólo archivo de plantilla y los fragmentos son los bloques definidos en ese archivo.

En PHP un tema es un directorio y los fragmentos son archivos de plantilla individuales en ese directorio.

.. _cookbook-form-customization-sidebar:

.. sidebar:: Saber cual bloque personalizar

    En este ejemplo, el nombre del fragmento personalizado es ``integer_widget`` debido a que deseas reemplazar el ``elemento gráfico`` HTML para todos los campos de tipo ``entero``. Si necesitas personalizar los campos ``textarea``, debes personalizar el ``textarea_widget``.

    Como puedes ver, el nombre del bloque es una combinación del tipo de campo y qué parte del campo se está reproduciendo (por ejemplo, ``widget``, ``label``, ``errores``, ``row``). Como tal, para personalizar cómo se reproducen los errores, tan sólo para campos de entrada ``text``, debes personalizar el fragmento ``text_errors``.

    Muy comúnmente, sin embargo, deseas personalizar cómo se muestran los errores en *todos* los campos. Puedes hacerlo personalizando el fragmento ``field_errors``. Este aprovecha la herencia del tipo de campo. Especialmente, ya que el tipo``text`` se extiende desde el tipo ``field``, el componente `form` busca el bloque del tipo específico (por ejemplo, ``text_errors``) antes de caer de nuevo al nombre del fragmento padre si no existe (por ejemplo, ``field_errors``).

    Para más información sobre este tema, consulta :ref:`form-template-blocks`.

.. _cookbook-form-theming-methods:

Tematizando formularios
-----------------------

Para ver el poder del tematizado de formularios, supongamos que deseas envolver todos los campos de entrada ``número`` con una etiqueta ``div``. La clave para hacerlo es personalizar el fragmento ``text_widget``.

Tematizando formularios en Twig
-------------------------------

Cuando personalizamos el bloque de campo de formulario en Twig, tienes dos opciones en *donde* puede vivir el bloque personalizado del formulario:

+--------------------------------------+-----------------------------------+-------------------------------------------+
| Método                               | Pros                              | Contras                                   |
+======================================+===================================+===========================================+
| Dentro de la misma plantilla que el  | Rápido y fácil                    | No se puede reutilizar en otra plantilla  |
| formulario                           |                                   |                                           |
+--------------------------------------+-----------------------------------+-------------------------------------------+
| Dentro de una plantilla separada     | Se puede reutilizar en muchas     | Requiere la creación de una plantilla     |
|                                      | plantillas                        | extra                                     |
+--------------------------------------+-----------------------------------+-------------------------------------------+

Ambos métodos tienen el mismo efecto, pero son mejores en diferentes situaciones.

.. _cookbook-form-twig-theming-self:

Método 1: Dentro de la misma plantilla que el formulario
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La forma más sencilla de personalizar el bloque ``integer_widget`` es personalizarlo directamente en la plantilla que realmente reproduce el formulario.

.. code-block:: html+jinja

    {% extends '::base.html.twig' %}

    {% form_theme form _self %}

    {% block integer_widget %}
        <div class="integer_widget">
            {% set type = type|default('number') %}
            {{ block('field_widget') }}
        </div>
    {% endblock %}

    {% block contenido %}
        {# reproduce el formulario #}

        {{ form_row(form.age) }}
    {% endblock %}

Al usar las etiquetas especiales ``{% form_theme form _self %}``, Twig busca dentro de la misma plantilla cualquier bloque de formulario a sustituir. Suponiendo que el campo ``form.edad`` es un campo de tipo ``entero``, cuando se reproduzca el elemento gráfico, utilizará el bloque personalizado ``integer_widget``.

La desventaja de este método es que los bloques personalizados del formulario no se pueden reutilizar en otros formularios reproducidos en otras plantillas. En otras palabras, este método es más útil cuando haces personalizaciones en forma que sean específicas a un único formulario en tu aplicación. Si deseas volver a utilizar una personalización a través de varios (o todos) los formularios de tu aplicación, lee la siguiente sección.

.. _cookbook-form-twig-separate-template:

Método 2: Dentro de una plantilla separada
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

También puedes optar por poner el bloque personalizado ``integer_widget`` del formulario en una plantilla completamente independiente. El código y el resultado final son el mismo, pero ahora puedes volver a utilizar la personalización de un formulario a través de muchas plantillas:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}

    {% block integer_widget %}
        <div class="integer_widget">
            {% set type = type|default('number') %}
            {{ block('field_widget') }}
        </div>
    {% endblock %}

Ahora que haz creado el bloque personalizado, es necesario decirle a Symfony que lo utilice. Dentro de la plantilla en la que estás reproduciendo tu formulario realmente, dile a Symfony que utilice la plantilla por medio de la etiqueta ``form_theme``:

.. _cookbook-form-twig-theme-import-template:

.. code-block:: html+jinja

    {% form_theme form 'AcmeDemoBundle:Form:fields.html.twig' %}

    {{ form_widget(form.edad) }}

Cuando se reproduzca el ``form.edad``, Symfony utilizará el bloque ``integer_widget`` de la nueva plantilla y la etiqueta ``input`` será envuelta en el elemento ``div`` especificado en el bloque personalizado.

.. _cookbook-form-php-theming:

Tematizando formularios en PHP
------------------------------

Cuando usas PHP como motor de plantillas, el único método para personalizar un fragmento es crear un nuevo archivo de plantilla - esto es similar al segundo método utilizado por Twig.

El archivo de plantilla se debe nombrar después del fragmento. Debes crear un archivo ``integer_widget.html.php`` a fin de personalizar el fragmento ``integer_widget``.

.. code-block:: html+php

    <!-- src/Acme/DemoBundle/Resources/views/Form/integer_widget.html.php -->

    <div class="integer_widget">
        <?php echo $view['form']->renderBlock('field_widget', array('type' => isset($type) ? $type : "number")) ?>
    </div>

Ahora que haz creado la plantilla del formulario personalizado, necesitas decirlo a Symfony para utilizarlo. Dentro de la plantilla en la que estás reproduciendo tu formulario realmente, dile a Symfony que utilice la plantilla por medio del método ayudante ``setTheme``:

.. _cookbook-form-php-theme-import-template:

.. code-block:: php

    <?php $view['form']->setTheme($formulario, array('AcmeDemoBundle:Form')) ;?>

    <?php $view['form']->widget($formulario['edad']) ?>

Al reproducir el elemento gráfico ``form.edad``, Symfony utilizará la plantilla personalizada ``integer_widget.html.php`` y la etiqueta ``input`` será envuelta en el elemento ``div``.

.. _cookbook-form-twig-import-base-blocks:

Refiriendo bloques del formulario base (específico de Twig)
-----------------------------------------------------------

Hasta ahora, para sustituir un bloque ``form`` particular, el mejor método consiste en copiar el bloque predeterminado desde `form_div_base.html.twig`_, pegarlo en una plantilla diferente y entonces, personalizarlo. En muchos casos, puedes evitarte esto refiriendo al bloque base cuando lo personalizas.

Esto se logra fácilmente, pero varía ligeramente dependiendo de si el bloque del formulario personalizado se encuentra en la misma plantilla que el formulario o en una plantilla separada.

Refiriendo bloques dentro de la misma plantilla que el formulario
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Importa los bloques añadiendo una etiqueta ``use`` en la plantilla donde estás reproduciendo el formulario:

.. code-block:: jinja

    {% use 'form_div_base.html.twig' with integer_widget as base_integer_widget %}

Ahora, cuando importes bloques desde `form_div_base.html.twig`_, el bloque ``integer_widget`` es llamado ``base_integer_widget``. Esto significa que cuando redefines el bloque ``integer_widget``, puedes referir el marcado predeterminado a través de ``base_integer_widget``:

.. code-block:: html+jinja

    {% block integer_widget %}
        <div class="integer_widget">
            {{ block('base_integer_widget') }}
        </div>
    {% endblock %}

Refiriendo bloques base desde una plantilla externa
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si tus personalizaciones del formulario viven dentro de una plantilla externa, puedes referir al bloque base con la función ``parent()`` de Twig:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}

    {% extends 'form_div_base.html.twig' %}

    {% block integer_widget %}
        <div class="integer_widget">
            {{ parent() }}
        </div>
    {% endblock %}

.. note::

    No es posible hacer referencia al bloque base cuando usas PHP como motor de plantillas. Tienes que copiar manualmente el contenido del bloque base a tu nuevo archivo de plantilla.

.. _cookbook-form-global-theming:

Personalizando toda tu aplicación
---------------------------------

Si deseas que una personalización en cierto formulario sea global en tu aplicación, lo puedes lograr haciendo las personalizaciones del formulario en una plantilla externa y luego importarla dentro de la configuración de tu aplicación:

Twig
~~~~

Al utilizar la siguiente configuración, los bloques personalizados del formulario dentro de la plantilla ``AcmeDemoBundle:Form:fields.html.twig`` se utilizarán globalmente al reproducir un formulario.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeDemoBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <twig:config ...>
                <twig:form>
                    <resource>AcmeDemoBundle:Form:fields.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php

        $contenedor->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'AcmeDemoBundle:Form:fields.html.twig',
             ))
            // ...
        ));

De forma predeterminada, Twig utiliza un diseño con *div* al reproducir formularios. Algunas personas, sin embargo, pueden preferir reproducir formularios en un diseño con *tablas*. Usa el recurso ``form_table_base.html.twig`` para utilizarlo como diseño:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources: ['form_table_base.html.twig']
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <twig:config ...>
                <twig:form>
                    <resource>form_table_base.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php

        // app/config/config.php

        $contenedor->loadFromExtension('twig', array(
            'form' => array('resources' => array(
                'form_table_base.html.twig',
             ))
            // ...
        ));

Si sólo quieres hacer el cambio en una plantilla, añade la siguiente línea a tu archivo de plantilla en lugar de agregar la plantilla como un recurso:

.. code-block:: html+jinja

	{% form_theme form 'form_table_base.html.twig' %}

Ten en cuenta que la variable ``form`` en el código anterior es la variable de la vista del formulario pasada a la plantilla.

PHP
~~~

Al utilizar la siguiente configuración, cualquier fragmento de formulario personalizado dentro del directorio ``src/Acme/DemoBundle/Resources/views/Form`` se usará globalmente al reproducir un formulario.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        framework:
            templating:
                form:
                    resources:
                        - 'AcmeDemoBundle:Form'
            # ...


    .. code-block:: xml

        <!-- app/config/config.xml -->

        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>AcmeDemoBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>


    .. code-block:: php

        // app/config/config.php

        // PHP
        $contenedor->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'AcmeDemoBundle:Form',
             )))
            // ...
        ));

De manera predeterminada, el motor PHP utiliza un diseño *div* al reproducir formularios. Algunas personas, sin embargo, pueden preferir reproducir formularios en un diseño con *tablas*. Utiliza el recurso ``FrameworkBundle:FormTable`` para utilizar este tipo de diseño:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        framework:
            templating:
                form:
                    resources:
                        - 'FrameworkBundle:FormTable'

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>FrameworkBundle:FormTable</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php

        // app/config/config.php

        $contenedor->loadFromExtension('framework', array(
            'templating' => array('form' =>
                array('resources' => array(
                    'FrameworkBundle:FormTable',
             )))
            // ...
        ));

Si sólo quieres hacer el cambio en una plantilla, añade la siguiente línea a tu archivo de plantilla en lugar de agregar la plantilla como un recurso:

.. code-block:: html+php

	<?php $view['form']->setTheme($formulario, array('FrameworkBundle:FormTable')); ?>

Ten en cuenta que la variable ``$formulario`` en el código anterior es la variable de la vista del formulario que pasaste a tu plantilla.

Cómo personalizar un campo individual
-------------------------------------

Hasta ahora, hemos visto diferentes formas en que puedes personalizar elementos gráficos de todos los tipos de campo de texto. También puedes personalizar campos individuales. Por ejemplo, supongamos que tienes dos campos ``text`` - ``nombre_de_pila`` y ``apellido`` -, pero sólo quieres personalizar uno de los campos. Esto se puede lograr personalizando un fragmento cuyo nombre es una combinación del atributo id del campo y cual parte del campo estás personalizando. Por ejemplo:

.. code-block:: html+jinja

    {% form_theme form _self %}

    {% block _product_name_widget %}
        <div class="text_widget">
            {{ block('field_widget' }}
        </div>
    {% endblock %}

    {{ form_widget(form.nombre) }}

.. code-block:: html+php

    <!-- Plantilla principal -->

    <?php echo $view['form']->setTheme($formulario, array('AcmeDemoBundle:Form')); ?>

    <?php echo $view['form']->widget($formulario['nombre']); ?>

    <!-- src/Acme/DemoBundle/Resources/views/Form/_product_name_widget.html.php -->

    <div class="text_widget">
          echo $view['form']->renderBlock('field_widget') ?>
    </div>

Aquí, el fragmento ``_product_nombre_widget`` define la plantilla a utilizar para el campo cuyo *id* es ``product_nombre`` (y nombre es ``product[nombre]``).

.. tip::

   La parte ``producto`` del campo es el nombre del formulario, la cual puedes ajustar manualmente o generar automáticamente a partir del nombre del tipo en el formulario (por ejemplo, ``ProductoType`` equivale a ``producto``). Si no estás seguro cual es el nombre del formulario, solo ve el código fuente del formulario generado.

También puedes sustituir el marcado de toda la fila de un campo usando el mismo método:

.. code-block:: html+jinja

    {% form_theme form _self %}

    {% block _product_name_row %}
        <div class="name_row">
            {{ form_label(form) }}
            {{ form_errors(form) }}
            {{ form_widget(form) }}
        </div>
    {% endblock %}

.. code-block:: html+php

    <!-- _product_name_row.html.php -->

    <div class="name_row">
        <?php echo $view['form']->label($formulario) ?>
        <?php echo $view['form']->errors($formulario) ?>
        <?php echo $view['form']->widget($formulario) ?>
    </div>

Otras personalizaciones comunes
-------------------------------

Hasta el momento, esta receta ha mostrado varias formas de personalizar una sola pieza de cómo se reproduce un formulario. La clave está en personalizar un fragmento específico que corresponde a la porción del formulario que deseas controlar (consulta :ref:`nombrando bloques de formulario <cookbook-form-customization-sidebar>`).

En las siguientes secciones, verás cómo puedes hacer varias personalizaciones de formulario comunes.
Para aplicar estas personalizaciones, utiliza uno de los dos métodos descritos en la sección :ref:`cookbook-form-twig-two-methods`.

Personalizando la salida de error
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::
    El componente `form` sólo se ocupa de la *forma* en que los errores de validación se reproducen, y no los mensajes de error de validación reales. Los mensajes de error están determinados por las restricciones de validación que apliques a tus objetos.
   Para más información, ve el capítulo :doc:`Validando </book/validation>`.

Hay muchas maneras de personalizar el modo en que se representan los errores cuando se envía un formulario con errores. Los mensajes de error de un campo se reproducen cuando se utiliza el ayudante ``form_errors``:

.. code-block:: jinja

    {{ form_errors(form.edad) }}

.. code-block:: php

    <?php echo $view['form']->errors($formulario['edad']); ?>

De forma predeterminada, los errores se representan dentro de una lista desordenada:

.. code-block:: html

    <ul>
        <li>Este campo es obligatorio</li>
    </ul>

Para redefinir cómo se reproducen los errores para *todos* los campos, simplemente copia, pega y personaliza el fragmento ``field_errors``.

.. code-block:: html+jinja

    {% block field_errors %}
    {% spaceless %}
        {% if errors|length > 0 %}
        <ul class="error_list">
            {% for error in errors %}
                <li>{{ error.messageTemplate|trans(error.messageParameters, 'validators') }}</li>
            {% endfor %}
        </ul>
        {% endif %}
    {% endspaceless %}
    {% endblock field_errors %}

.. code-block:: html+php

    <!-- fields_errors.html.php -->

    <?php if ($errors): ?>
        <ul class="error_list">
            <?php foreach ($errors as $error): ?>
                <li><?php echo $view['translator']->trans(
                    $error->getMessageTemplate(),
                    $error->getMessageParameters(),
                    'validators'
                ) ?></li>
            <?php endforeach; ?>
        </ul>
    <?php endif ?>

.. tip::
    Consulta :ref:`cookbook-form-theming-methods` para ver cómo aplicar esta personalización.

También puedes personalizar la salida de error de sólo un tipo de campo específico.
Por ejemplo, algunos errores que son más globales en tu formulario (es decir, no específicos a un solo campo) se reproducen por separado, por lo general en la parte superior de tu formulario:

.. code-block:: jinja

    {{ form_errors(form) }}

.. code-block:: php

    <?php echo $view['form']->render($formulario); ?>

Para personalizar *sólo* el formato utilizado por estos errores, sigue las mismas instrucciones que el anterior, pero ahora llamamos al bloque ``form_errors`` (Twig) / el archivo ``form_errors.html.php`` (PHP). Ahora, al reproducir errores del tipo ``form``, se utiliza el fragmento personalizado en lugar del ``field_errors`` predeterminado.

Personalizando una "fila del formulario"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cuando consigas manejarla, la forma más fácil para reproducir un campo de formulario es a través de la función ``form_row``, la cual reproduce la etiqueta, errores y el elemento gráfico HTML de un campo. Para personalizar el formato utilizado para reproducir *todas* las filas de los campos del formulario, redefine el fragmento ``field_row``. Por ejemplo, supongamos que deseas agregar una clase al elemento ``div`` alrededor de cada fila:

.. code-block:: html+jinja

    {% block field_row %}
        <div class="form_row">
            {{ form_label(form) }}
            {{ form_errors(form) }}
            {{ form_widget(form) }}
        </div>
    {% endblock field_row %}

.. code-block:: html+php

    <!-- field_row.html.php -->

    <div class="form_row">
        <?php echo $view['form']->label($formulario) ?>
        <?php echo $view['form']->errors($formulario) ?>
        <?php echo $view['form']->widget($formulario) ?>
    </div>

.. tip::
    Consulta :ref:`cookbook-form-theming-methods` para ver cómo aplicar esta personalización.

Añadiendo un asterisco "Required" a las etiquetas de campo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si deseas denotar todos los campos obligatorios con un asterisco requerido (``*``), lo puedes hacer personalizando el fragmento ``field_label``.

En Twig, si estás haciendo la personalización del formulario dentro de la misma plantilla que tu formulario, modifica la etiqueta ``use`` y añade lo siguiente:

.. code-block:: html+jinja

    {% use 'form_div_base.html.twig' with field_label as base_field_label %}

    {% block field_label %}
        {{ block('base_field_label') }}

        {% if required %}
            <span class="required" titulo="This field is required">*</span>
        {% endif %}
    {% endblock %}

En Twig, si estás haciendo la personalización del formulario dentro de una plantilla separada, utiliza lo siguiente:

.. code-block:: html+jinja

    {% extends 'form_div_base.html.twig' %}

    {% block field_label %}
        {{ parent() }}

        {% if required %}
            <span class="required" titulo="This field is required">*</span>
        {% endif %}
    {% endblock %}

Cuando usas PHP como motor de plantillas tienes que copiar el contenido desde la plantilla original:

.. code-block:: html+php

    <!-- field_label.html.php -->

    <!-- contenido original -->
    <label for="<?php echo $view->escape($id) ?>" <?php foreach($attr as $k => $v) { printf('%s="%s" ', $view->escape($k), $view->escape($v)); } ?>><?php echo $view->escape($view['translator']->trans($label)) ?></label>

    <!-- personalización -->
    <?php if ($required) : ?>
        <span class="required" titulo="Este campo es obligatorio">*</span>
    <?php endif ?>

.. tip::
    Consulta :ref:`cookbook-form-theming-methods` para ver cómo aplicar esta personalización.

Añadiendo mensajes de "ayuda"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

También puedes personalizar los elementos gráficos del formulario para que tengan un mensaje de "ayuda" opcional.

En Twig, si estás haciendo la personalización del formulario dentro de la misma plantilla que tu formulario, modifica la etiqueta ``use`` y añade lo siguiente:

.. code-block:: html+jinja

    {% use 'form_div_base.html.twig' with field_widget as base_field_widget %}

    {% block field_widget %}
        {{ block('base_field_widget') }}

        {% if help is defined %}
            <span class="help">{{ help }}</span>
        {% endif %}
    {% endblock %}

En Twig, si estás haciendo la personalización del formulario dentro de una plantilla separada, utiliza lo siguiente:

.. code-block:: html+jinja

    {% extends 'form_div_base.html.twig' %}

    {% block field_widget %}
        {{ parent() }}

        {% if help is defined %}
            <span class="help">{{ help }}</span>
        {% endif %}
    {% endblock %}

Cuando usas PHP como motor de plantillas tienes que copiar el contenido desde la plantilla original:

.. code-block:: html+php

    <!-- field_widget.html.php -->

    <!-- contenido original -->
    <input
        type="<?php echo isset($type) ? $view->escape($type) : "text" ?>"
        value="<?php echo $view->escape($value) ?>"
        <?php echo $view['form']->renderBlock('attributes') ?>
    />

    <!-- Personalización -->
    <?php if (isset($help)) : ?>
        <span class="help"><?php echo $view->escape($help) ?></span>
    <?php endif ?>

Para reproducir un mensaje de ayuda debajo de un campo, pásalo en una variable ``help``:

.. code-block:: jinja

    {{ form_widget(form.titulo, { 'help': 'foobar' }) }}

.. code-block:: php

    <?php echo $view['form']->widget($formulario['titulo'], array('help' => 'foobar')) ?>

.. tip::
    Consulta :ref:`cookbook-form-theming-methods` para ver cómo aplicar esta personalización.

.. _`form_div_base.html.twig`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_base.html.twig
..