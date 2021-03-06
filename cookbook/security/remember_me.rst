Cómo agregar la funcionalidad "recuérdame" al inicio de sesión
==============================================================

Una vez que un usuario está autenticado, normalmente sus credenciales se almacenan en la sesión. Esto significa que cuando termina la sesión se desechará la sesión y tienes que proporcionar de nuevo tus datos de acceso la próxima vez que desees acceder a la aplicación. Puedes permitir a tus usuarios que puedan optar por permanecer conectados durante más tiempo del que dure la sesión con una ``cookie`` con la opción ``remember_me`` del cortafuegos. 
El cortafuegos necesita tener configurada una clave secreta, la cual se utiliza para cifrar el contenido de la ``cookie``. También tiene varias opciones con los valores predeterminados que se muestran a continuación:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        firewalls:
            main:
                remember_me:
                    key:      aSecretKey
                    lifetime: 3600
                    path:     /
                    domain:   ~ # El valor predeterminado es el dominio actual de $_SERVER

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <remember-me
                    key="aSecretKey"
                    lifetime="3600"
                    path="/"
                    domain="" <!-- El valor predeterminado es el dominio actual de $_SERVER -->
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $contenedor->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('remember_me' => array(
                    'key'                     => '/login_check',
                    'lifetime'                => 3600,
                    'path'                    => '/',
                    'domain'                  => '', // El valor predeterminado es el dominio actual de $_SERVER
                )),
            ),
        ));

Es buena idea ofrecer al usuario la opción de utilizar o no la funcionalidad recuérdame, ya que no siempre es adecuada. La forma habitual de hacerlo consiste en añadir una casilla de verificación en el formulario de acceso. Al dar a la casilla de verificación el nombre ``_remember_me``, la ``cookie`` se ajustará automáticamente cuando la casilla esté marcada y el usuario inicia sesión satisfactoriamente. Por lo tanto, tu formulario de acceso específico en última instancia, podría tener este aspecto:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/SecurityBundle/Resources/views/Security/login.html.twig #}
        {% if error %}
            <div>{{ error.message }}</div>
        {% endif %}

        <form action="{{ path('login_check') }}" method="post">
            <label for="nombreusuario">Nombreusuario:</label>
            <input type="text" id="nombreusuario" name="_username" value="{{ last_username }}" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="checkbox" id="remember_me" name="_remember_me" checked />
            <label for="remember_me">Keep me logged in</label>

            <input type="submit" name="login" />
        </form>

    .. code-block:: html+php

        <?php // src/Acme/SecurityBundle/Resources/views/Security/login.html.php ?>
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="nombreusuario">Nombreusuario:</label>
            <input type="text" id="nombreusuario" 
                   name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="checkbox" id="remember_me" name="_remember_me" checked />
            <label for="remember_me">Keep me logged in</label>

            <input type="submit" name="login" />
        </form>

El usuario entonces, se registra automáticamente en las subsecuentes visitas, mientras que la ``cookie`` sea válida.

Forzando al usuario a volver a autenticarse antes de acceder a ciertos recursos
-------------------------------------------------------------------------------

Cuando el usuario vuelve a tu sitio, se autentica automáticamente en función de la información almacenada en la ``cookie`` recuérdame. Esto permite al usuario acceder a recursos protegidos como si el usuario se hubiera autenticado en realidad al visitar el sitio.

En algunos casos, sin embargo, puedes obligar al usuario a realmente volver a autenticarse antes de acceder a ciertos recursos. Por ejemplo, podrías permitir a un usuario de "recuérdame" ver la información básica de la cuenta, pero luego obligarlo a volver a autenticarse realmente antes de modificar dicha información.

El componente de seguridad proporciona una manera fácil de hacerlo. Además de los roles asignados expresamente, a los usuarios se les asigna automáticamente uno de los siguientes roles, dependiendo de cómo se haya autenticado:

* ``IS_AUTHENTICATED_ANONYMOUSLY`` - asignado automáticamente a un usuario que está en una parte del sitio protegida por el cortafuegos, pero que no ha iniciado sesión. 
  Esto sólo es posible si se le ha permitido el acceso anónimo.

* ``IS_AUTHENTICATED_REMEMBERED`` - asignado automáticamente a un usuario autenticado a través de una ``cookie`` recuérdame.

* ``IS_AUTHENTICATED_FULLY`` - asignado automáticamente a un usuario que haya proporcionado sus datos de acceso durante la sesión actual.

Las puedes utilizar para controlar el acceso más allá de los roles asignados explícitamente.

.. note::

    Si tienes el rol ``IS_AUTHENTICATED_REMEMBERED``, entonces también tienes el rol ``IS_AUTHENTICATED_ANONYMOUSLY``. Si tienes el rol ``IS_AUTHENTICATED_FULLY``, entonces también tienes los otros dos roles. En otras palabras, estos roles representan tres niveles de "fortaleza" autenticación incremental.

Puedes utilizar estos roles adicionales para un control más granular sobre el acceso a partes de un sitio. Por ejemplo, posiblemente desees que el usuario pueda ver su cuenta en ``/cuenta`` cuando está autenticado por ``cookie``, pero tiene que proporcionar sus datos de acceso para poder editar la información de la cuenta. Lo puedes hacer protegiendo acciones específicas del controlador usando estos roles. La acción de edición en el controlador se puede proteger usando el servicio contexto. 

En el siguiente ejemplo, la acción sólo es permitida si el usuario tiene el rol ``IS_AUTHENTICATED_FULLY``.

.. code-block:: php

    use Symfony\Component\Security\Core\Exception\AccessDeniedException
    // ...

    public function editAction()
    {
        if (false === $this->get('security.context')->isGranted(
            'IS_AUTHENTICATED_FULLY'
        )) {
            throw new AccessDeniedException();
        }

        // ...
    }

También puedes optar por instalar y utilizar el opcional ``SecurityExtraBundle``, el cual puede proteger tu controlador usando anotaciones:

.. code-block:: php

    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Secure(roles="IS_AUTHENTICATED_FULLY")
     */
    public function editAction($nombre)
    {
        // ...
    }

.. tip::

    Si, además, hubiera un control de acceso en la configuración de seguridad que requiere que el usuario tenga un rol ``ROLE_USER`` a fin de acceder a cualquier área de la cuenta, entonces tendríamos la siguiente situación:

    * Si un usuario no autenticado (o un usuario autenticado anónimamente) intenta acceder al área de la cuenta, el usuario se tendrá que autenticar.
    
    * Una vez que el usuario ha introducido su nombre de usuario y contraseña, asumiendo que el usuario recibe el rol ``ROLE_USER`` de tu configuración, el usuario tendrá el rol ``IS_AUTHENTICATED_FULLY`` y podrá acceder a cualquier página en la sección de cuenta, incluyendo el controlador ``editAction``.

    * Si termina la sesión del usuario, cuando el usuario vuelve al sitio, podrá acceder a cada página de la cuenta - a excepción de la página de edición - sin verse obligado a volver a autenticarse. Sin embargo, cuando intenta acceder al controlador ``editAction``, se verá obligado a volver a autenticarse, ya que no está, sin embargo, totalmente autenticado.

Para más información sobre proteger servicios o métodos de esta manera, consulta :doc:`/cookbook/security/securing_services`.