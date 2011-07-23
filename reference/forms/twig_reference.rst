.. index::
   single: Formularios; Referencia de funciones de formulario Twig

Referencia de funciones de formulario en plantillas Twig
========================================================

Este manual de referencia cubre todas las posibles funciones Twig disponibles para reproducir formularios. Hay varias funciones diferentes disponibles, y cada una es responsable de representar una parte diferente de un formulario (por ejemplo, etiquetas, errores, elementos gráficos, etc.).

form_label(form.nombre, label, variables)
-----------------------------------------

Reproduce la etiqueta para el campo dado. Si lo deseas, puedes pasar como segundo argumento la etiqueta específica que deseas mostrar.

.. code-block:: jinja

    {{ form_label(form.nombre) }}

    {# Las dos siguientes sintaxis son equivalentes #}
    {{ form_label(form.nombre, 'Tu nombre', { 'attr': {'class': 'foo'} }) }}
    {{ form_label(form.nombre, null, { 'label': 'Tu nombre', 'attr': {'class': 'foo'} }) }}

form_errors(form.nombre)
------------------------

Representa los errores para el campo dado.

.. code-block:: jinja

    {{ form_errors(form.nombre) }}

    {# render any "global" errors #}
    {{ form_errors(form) }}

form_widget(form.nombre, variables)
-----------------------------------

Representa el elemento gráfico HTML de un campo determinado. Si aplicas este a todo el formulario o a la colección de campos, reproducirá cada fila subyacente del formulario.

.. code-block:: jinja

    {# reproduce un elemento gráfico, pero le agrega una clase "foo" #}
    {{ form_widget(form.nombre, { 'attr': {'class': 'foo'} }) }}

El segundo argumento de ``form_widget`` es un conjunto de variables. La variable más común es ``attr``, que es una matriz de atributos HTML que puedes aplicar al elemento gráfico HTML. En algunos casos, ciertos tipos también tienen otras opciones relacionadas con la plantilla que les puedes pasar. Estas se explican en base a tipo por tipo.

form_row(form.nombre, variables)
--------------------------------

Representa la "fila" (row) de un campo determinado, el cual es la combinación de la etiqueta del campo, los errores y el elemento gráfico.

.. code-block:: jinja

    {# reproduce una fila de un campo, pero añade una clase "foo" #}
    {{ form_row(form.nombre, { 'label': 'foo' }) }}

El segundo argumento de ``form_row`` es un arreglo de variables. Las plantillas de Symfony sólo permite redefinir la etiqueta como muestra el ejemplo anterior.

form_rest(form, variables)
--------------------------

Esto reproduce todos los campos que aún no se han presentado en el formulario dado.
Es buena idea tenerlo siempre en alguna parte dentro del formulario ya que debe representar los campos ocultos por ti y los campos que se te olvide representar (puesto que va a representar el campo para ti).

.. code-block:: jinja

    {{ form_rest(form) }}

form_enctype(form)
------------------

Si el formulario contiene al menos un campo de carga de archivos, esta reproducirá el atributo requerido ``enctype=multipart/form-data"``. Siempre es una buena idea incluirlo en tu etiqueta de formulario:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>