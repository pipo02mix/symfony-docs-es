``preferred_choices``
~~~~~~~~~~~~~~~~~~~~~

**tipo**: ``array`` **predeterminado**: ``array()``

Si se especifica esta opción, entonces un subconjunto de todas las opciones se trasladará a la parte superior del menú de selección. Lo siguiente debe mover la opción "Baz" a la parte superior, con una separación visual entre esta y el resto de las opciones::

    $builder->add('foo_choices', 'choice', array(
        'choices' => array('foo' => 'Foo', 'bar' => 'Bar', 'baz' => 'Baz'),
        'preferred_choices' => array('baz'),
    ));

Ten en cuenta que las opciones preferidas sólo son útiles cuando se reproducen como un elemento ``select`` (es decir, ``expanded`` es ``false``). Las opciones preferidas y las opciones normales están separadas visualmente por un conjunto de líneas punteadas (es decir, ``-------------------``).
Esto se puede personalizar cuando reproduzcas el campo:

.. configuration-block::

    .. code-block:: jinja

        {{ form_widget(form.foo_choices, { 'separator': '=====' }) }}

    .. code-block:: php

        <?php echo $view['form']->widget($formulario['foo_choices'], array('separator' => '=====')) ?>
