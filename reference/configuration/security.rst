.. index::
   single: Seguridad; Referencia de configuración

Referencia en configurando ``Security``
=======================================

El sistema de seguridad es una de las piezas más poderosas de Symfony2, y en gran medida se puede controlar por medio de su configuración.

Configuración predeterminada completa
-------------------------------------

La siguiente es la configuración predeterminada para el sistema de seguridad completo.
Cada parte se explica en la siguiente sección.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            access_denied_url: /foo/error403

            always_authenticate_before_granting: false

            # la estrategia puede ser: none, migrate, invalidate
            session_fixation_strategy: migrate

            access_decision_manager:
                strategy: affirmative
                allow_if_all_abstain: false
                allow_if_equal_granted_denied: true

            acl:
                connection: default # algún nombre configurado en la sección doctrine.dbal
                tables:
                    class: acl_classes
                    entry: acl_entries
                    object_identity: acl_object_identities
                    object_identity_ancestors: acl_object_identity_ancestors
                    security_identity: acl_security_identities
                cache:
                    id: service_id
                    prefix: sf2_acl_
                voter:
                    allow_if_object_identity_unavailable: true

            encoders:
                somename:
                    class: Acme\DemoBundle\Entity\User
                Acme\DemoBundle\Entity\User: sha512
                Acme\DemoBundle\Entity\User: plaintext
                Acme\DemoBundle\Entity\User:
                    algorithm: sha512
                    encode_as_base64: true
                    iterations: 5000
                Acme\DemoBundle\Entity\User:
                    id: my.custom.encoder.service.id

            providers:
                memory:
                    name: memory
                    users:
                        foo: { password: foo, roles: ROLE_USER }
                        bar: { password: bar, roles: [ROLE_USER, ROLE_ADMIN] }
                entity:
                    entity: { class: SecurityBundle:User, property: nombreusuario }

            factories:
                MyFactory: %kernel.root_dir%/../src/Acme/DemoBundle/Resources/config/security_factories.xml

            firewalls:
                somename:
                    pattern: .*
                    request_matcher: some.service.id
                    access_denied_url: /foo/error403
                    access_denied_handler: some.service.id
                    entry_point: some.service.id
                    provider: name
                    context: name
                    stateless: false
                    x509:
                        provider: name
                    http_basic:
                        provider: name
                    http_digest:
                        provider: name
                    form_login:
                        check_path: /login_check
                        login_path: /login
                        use_forward: false
                        always_use_default_target_path: false
                        default_target_path: /
                        target_path_parameter: _target_path
                        use_referer: false
                        failure_path: /foo
                        failure_forward: false
                        failure_handler: some.service.id
                        success_handler: some.service.id
                        username_parameter: _username
                        password_parameter: _password
                        csrf_parameter: _csrf_token
                        csrf_page_id: form_login
                        csrf_provider: my.csrf_provider.id
                        post_only: true
                        remember_me: false
                    remember_me:
                        token_provider: name
                        key: someS3cretKey
                        name: NameOfTheCookie
                        lifetime: 3600 # in seconds
                        path: /foo
                        domain: somedomain.foo
                        secure: true
                        httponly: true
                        always_remember_me: false
                        remember_me_parameter: _remember_me
                    logout:
                        path:   /logout
                        target: /
                        invalidate_session: false
                        delete_cookies:
                            a: { path: null, domain: null }
                            b: { path: null, domain: null }
                        handlers: [some.service.id, another.service.id]
                        success_handler: some.service.id
                    anonymous: ~

            access_control:
                -
                    path: ^/foo
                    host: mydomain.foo
                    ip: 192.0.0.0/8
                    roles: [ROLE_A, ROLE_B]
                    requires_channel: https

            role_hierarchy:
                ROLE_SUPERADMIN: ROLE_ADMIN
                ROLE_SUPERADMIN: 'ROLE_ADMIN, ROLE_USER'
                ROLE_SUPERADMIN: [ROLE_ADMIN, ROLE_USER]
                anything: { id: ROLE_SUPERADMIN, value: 'ROLE_USER, ROLE_ADMIN' }
                anything: { id: ROLE_SUPERADMIN, value: [ROLE_USER, ROLE_ADMIN] }

.. _reference-security-firewall-form-login:

Configurando el formulario de acceso
------------------------------------

Cuando usas el escucha de autenticación ``form_login`` bajo un cortafuegos, hay varias opciones comunes para experimentar en la configuración del "formulario de acceso":

Procesando el formulario de acceso
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*   ``login_path`` (tipo: ``string``, predeterminado: ``/login``) Esta es la URL a que el usuario será redirigido (a menos que ``use_forward`` se haya fijado en ``true``) cuando él/ella intente acceder a un recurso protegido, pero no está totalmente autenticado.

    Esta **URL** debe ser accesible por un usuario normal, no autenticado, de lo contrario puede crear un bucle de redireccionamiento. Para más información, consulta ":ref:`Evitando errores comunes <book-security-common-pitfalls>`".

*   ``check_path`` (tipo: ``string``, predeterminado: ``/login_check``) Esta es la URL en la cual se debe presentar el formulario de acceso. El cortafuegos intercepta cualquier petición (sólo las peticiones ``POST``, por omisión) a esta URL y procesa las credenciales presentadas.
    
    Asegúrate de que esta dirección está cubierta por el cortafuegos principal (es decir, no crees un servidor de seguridad independiente sólo para la URL ``check_path``).

*   ``use_forward`` (tipo: ``Boolean``, predeterminado: ``false``) Si deseas que el usuario sea remitido al formulario de acceso en vez de ser redirigido, fija esta opción a ``true``.

*   ``username_parameter`` (tipo: ``string``, predeterminado: ``_username``) Este es el nombre del campo que debes dar al campo *nombre de usuario* de tu formulario de acceso. Cuando se presenta el formulario a ``check_path``, el sistema de seguridad buscará un parámetro POST con este nombre.

*   ``password_parameter`` (tipo: ``string``, predeterminado: ``_password``) Este es el nombre del campo que debes dar al campo de la *contraseña* de tu formulario de acceso. Cuando se presenta el formulario a ``check_path``, el sistema de seguridad buscará un parámetro POST con este nombre.

*   ``post_only`` (tipo: ``Boolean``, predeterminado: ``true``) De forma predeterminada, debes enviar tu formulario de acceso a la URL ``check_path`` como una petición POST. Al establecer esta opción a ``true``, puedes enviar una petición GET a la URL ``check_path``.

Redirigiendo después del inicio de sesión
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* ``always_use_default_target_path`` (tipo: ``Boolean``, predeterminado: ``false``)
* ``default_target_path`` (tipo: ``string``, predeterminado: ``/``)
* ``target_path_parameter`` (tipo: ``string``, predeterminado: ``_target_path``)
* ``use_referer`` (tipo: ``Boolean``, predeterminado: ``false``)
