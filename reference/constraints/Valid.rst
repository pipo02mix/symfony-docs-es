Valid
=====

Marca un objeto asociado para que se valide a sí mismo.

.. code-block:: yaml

    properties:
        domicilio:
            - Valid: ~

Ejemplo: Valida objetos gráficos
--------------------------------

Esta restricción ayuda a validar todo objeto gráfico. El siguiente ejemplo, crea dos clases ``Autor`` y ``Domicilio`` donde ambas tienen restricciones en sus propiedades. Además, ``Autor`` almacena una instancia de ``Domicilio`` en la propiedad ``$domicilio``.

.. code-block:: php

    // src/Acme/HolaBundle/Domicilio.php
    class Domicilio
    {
        protected $street;
        protected $zipCode;
    }

.. code-block:: php

    // src/Acme/HolaBundle/Autor.php
    class Autor
    {
        protected $nombreDePila;
        protected $apellido;
        protected $domicilio;
    }

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Domicilio:
            properties:
                street:
                    - NotBlank: ~
                zipCode:
                    - NotBlank: ~
                    - MaxLength: 5

        Acme\HolaBundle\Autor:
            properties:
                nombreDePila:
                    - NotBlank: ~
                    - MinLength: 4
                apellido:
                    - NotBlank: ~

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Domicilio">
            <property name="street">
                <constraint name="NotBlank" />
            </property>
            <property name="zipCode">
                <constraint name="NotBlank" />
                <constraint name="MaxLength">5</constraint>
            </property>
        </class>

        <class name="Acme\HolaBundle\Autor">
            <property name="nombreDePila">
                <constraint name="NotBlank" />
                <constraint name="MinLength">4</constraint>
            </property>
            <property name="apellido">
                <constraint name="NotBlank" />
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Domicilio.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Domicilio
        {
            /**
             * @Assert\NotBlank()
             */
            protected $street;

            /**
             * @Assert\NotBlank
             * @Assert\MaxLength(5)
             */
            protected $zipCode;
        }

        // src/Acme/HolaBundle/Autor.php
        class Autor
        {
            /**
             * @Assert\NotBlank
             * @Assert\MinLength(4)
             */
            protected $nombreDePila;

            /**
             * @Assert\NotBlank
             */
            protected $apellido;
            
            protected $domicilio;
        }

    .. code-block:: php

        // src/Acme/HolaBundle/Domicilio.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MaxLength;

        class Domicilio
        {
            protected $street;

            protected $zipCode;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('street', new NotBlank());
                $metadatos->addPropertyConstraint('zipCode', new NotBlank());
                $metadatos->addPropertyConstraint('zipCode', new MaxLength(5));
            }
        }

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class Autor
        {
            protected $nombreDePila;

            protected $apellido;

            protected $domicilio;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('nombreDePila', new NotBlank());
                $metadatos->addPropertyConstraint('nombreDePila', new MinLength(4));
                $metadatos->addPropertyConstraint('apellido', new NotBlank());
            }
        }

Con esta asignación puedes validar con éxito un autor con una dirección no válida. Para evitarlo, añade la restricción ``Valid`` a la propiedad ``$domicilio``.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Autor:
            properties:
                domicilio:
                    - Valid: ~

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Autor">
            <property name="domicilio">
                <constraint name="Valid" />
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /* ... */

            /**
             * @Assert\Valid
             */
            protected $domicilio;
        }

    .. code-block:: php

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Valid;

        class Autor
        {
            protected $domicilio;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('domicilio', new Valid());
            }
        }

Si ahora validas a un autor con una dirección no válida, puedes ver que la validación de los campos ``Domicilio`` fracasa.

    Acme\HolaBundle\Autor.domicilio.zipCode:
    Este valor es demasiado largo. Debe tener 5 caracteres o menos
