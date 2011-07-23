API estable de Symfony2
=======================

La API estable de Symfony2 es un subconjunto de todos los métodos públicos de Symfony2 (componentes y paquetes básicos) que comparten las siguientes propiedades:

* El nombre del espacio de nombres y clase no va a cambiar;
* El nombre del método no va a cambiar;
* La firma del método (argumentos y tipo del valor de retorno) no va a cambiar;
* La semántica del método no va a cambiar.

Sin embargo, la imprementación en sí misma puede cambiar. El único caso válido para un cambio en la API estable es con el fin de corregir un problema de seguridad.

.. note::

    La API estable se basa en una lista blanca. Por lo tanto, todo lo no mencionado explícitamente en este documento no forma parte de la API estable.

.. note::

    Se trata de un trabajo en curso y la lista definitiva se publicará cuando Symfony2 final sea liberado. Mientras tanto, si piensas que algunos métodos merecen estar en esta lista, tienes que iniciar un debate en la lista de correo de los desarrolladores de Symfony.

.. tip::

    Cualquier método que forma parte de la API estable está marcado como tal en la página web de la API de Symfony2 (tiene la anotación ``@stable``).

.. tip::

    Cualquier paquete de terceros también deberá publicar su propia API estable.

Componente ``HttpKernel``
-------------------------

* HttpKernelInterface:: :method:`Symfony\\Component\\HttpKernel\\HttpKernelInterface::handle`
