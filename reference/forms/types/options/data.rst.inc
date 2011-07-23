``data``
~~~~~~~~

**tipo**: mixed **predeterminado**: De manera predeterminada al campo del objeto subyacente (si existe)

Cuando creas un formulario, cada campo inicialmente muestra el valor de la propiedad correspondiente al objeto del dominio del formulario (si está ligado un objeto al formulario). Si sólo deseas cambiar el valor inicial para el formulario y campo individual, lo puedes establecer en la opción de ``data``::

    $builder->add('token', 'hidden', array(
        'data' => 'abcdef',
    ));


