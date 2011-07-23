*   ``disabled`` [tipo: Boolean, predeterminado: false] Si no deseas que el usuario pueda modificar el valor de un campo, puedes establecer la opción ``disabled`` en ``true``. Cualquier valor presentado será omitido.

    .. code-block:: php

        use Symfony\Component\Form\TextField

        $field = new TextField('status', array(
            'data' => 'Old data',
            'disabled' => true,
        ));
        $field->submit('New data');

        // imprime el "dato antiguo"
        echo $field->getData();


