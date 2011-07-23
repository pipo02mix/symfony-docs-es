``invalid_message``
~~~~~~~~~~~~~~~~~~~

**tipo**: ``string`` **predeterminado**: ``This value is not valid``

Este es el mensaje de error de validación utilizado cuando se determinan los datos ingresados mediante la validación interna de un tipo de campo. Esto puede suceder, por ejemplo, si el usuario ingresa una cadena en un campo :doc:`time</reference/forms/types/time>` el cual no la puede convertir en una hora real. Para los mensajes de validación normales (como cuando se establece una longitud mínima en un campo), fija los mensajes de validación con tus reglas de validación (:ref:`referencia <book-validation-constraint-configuration>`).