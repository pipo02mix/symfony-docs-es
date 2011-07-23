Añadiendo un "valor vacío"
--------------------------

Si estás utilizando la versión no ampliada del elemento tipo (es decir, una etiqueta ``select``) y te gustaría tener una entrada en blanco (por ejemplo, "Elija una opción") en la parte superior de la caja de selección, lo puedes hacer de la siguiente manera:

* Fija la opción ``multiple`` a falso;

* Fija la opción ``required`` a falso o pásala explícitamente en la opción ``empty_value`` en la plantilla (como se muestra a continuación).

Si ``requerido`` es falso, mostrará una opción en blanco en la parte superior del cuadro de selección. Para personalizar lo que dice la entrada (o si fijaste ``required`` como ``true``), agrega lo siguiente cuando reproduzcas el campo:

.. configuration-block::

    .. code-block:: jinja

        {{ form_widget(form.foo_choices, { 'empty_value': 'Choose an option' }) }}

    .. code-block:: php

        <?php echo $view['form']->widget($formulario['foo_choices'], array('empty_value' => 'Choose an option')) ?>
