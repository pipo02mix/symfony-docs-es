.. index::
   single: Pruebas; Autenticación HTTP

Cómo simular autenticación HTTP en una prueba funcional
=======================================================

Si tu aplicación necesita autenticación HTTP, pasa el nombre de usuario y contraseña como variables del servidor a ``createClient()``::

    $cliente = static::createClient(array(), array(
        'PHP_AUTH_USER' => 'username',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));

También lo puedes sustituir en base a la petición::

    $cliente->request('DELETE', '/post/12', array(), array(
        'PHP_AUTH_USER' => 'username',
        'PHP_AUTH_PW'   => 'pa$$word',
    ));
