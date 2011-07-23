``invalid_message_parameters``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**tipo**: ``array`` **predeterminado**: ``array()``

Al establecer la opción ``invalid_message``, posiblemente sea necesario que incluyas algunas variables en la cadena. Esto se puede lograr agregando marcadores de posición y variables en esa opción::

    $builder->add('algún_campo', 'algún_tip', array(
        // ...
        'invalid_message'            => 'Introdujiste un valor no válido - este debe incluir %num% letras',
        'invalid_message_parameters' => array('%num%' => 6),
    ));