``label``
~~~~~~~~~

**tipo**: ``string`` **predeterminado**: La etiqueta es "adivinada" a partir del nombre del campo

Establece la etiqueta que se utilizará al reproducir el campo. La etiqueta también se puede fijar directamente dentro de la plantilla:

.. code-block:: jinja

    {{ render_label(form.nombre, 'Tu nombre') }}