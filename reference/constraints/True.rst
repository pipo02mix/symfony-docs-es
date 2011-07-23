True
====

Valida que un valor es ``true``.

.. code-block:: yaml

    properties:
        termsAccepted:
            - True: ~

Opciones
--------

* ``message``: El mensaje de error si la validación falla

Ejemplo
-------

Esta restricción es muy útil para ejecutar lógica de validación personalizada. Puedes poner la lógica en un método que devuelva ``true`` o ``false``.

.. code-block:: php

    // Acme/HolaBundle/Autor.php
    class Autor
    {
        protected $token;

        public function isTokenValid()
        {
            return $this->token == $this->generateToken();
        }
    }

A continuación, puedes limitar este método con ``True``.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Autor:
            getters:
                tokenValid:
                    - True: { message: "The token is invalid" }

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Autor">
            <getter name="tokenValid">
                <constraint name="True">
                    <option name="message">The token is invalid</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            protected $token;

            /**
             * @Assert\True(message = "The token is invalid")
             */
            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

    .. code-block:: php

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;

        class Autor
        {
            protected $token;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addGetterConstraint('tokenValid', new True(array(
                    'message' => 'The token is invalid',
                )));
            }

            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

Si la validación de este método falla, verás un mensaje similar a este:

.. code-block:: text

    Acme\HolaBundle\Autor.tokenValid:
        Este valor no debe ser nulo
