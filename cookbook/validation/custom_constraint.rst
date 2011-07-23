.. index::
   single: Validación; Restricciones personalizadas

Cómo crear una restricción de validación personalizada
------------------------------------------------------

Puedes crear una restricción personalizada extendiendo de la clase base "constraint", :class:`Symfony\\Component\\Validator\\Constraint`. Las opciones para tu restricción se representan como propiedades públicas de la clase 'constraint'. Por ejemplo, la restricción :doc:`URL </reference/constraints/Url>` incluye las propiedades ``message`` y ``protocols``:

.. code-block:: php

    namespace Symfony\Component\Validator\Constraints;

    class Url extends \Symfony\Component\Validator\Constraint
    {
        public $message = 'This value is not a valid URL';
        public $protocols = array('http', 'https', 'ftp', 'ftps');
    }

Como puedes ver, una clase de restricción es bastante mínima. La validación real la realiza otra clase "validador de restricción". La clase validador de restricción se especifica por el método de la restricción ``validatedBy()``, que por omisión incluye alguna lógica simple:

.. code-block:: php

    // en la clase base Symfony\Component\Validator\Constraint
    public function validatedBy()
    {
        return get_class($this).'Validator';
    }

En otras palabras, si creas una ``restricción`` personalizada (por ejemplo, ``MyConstraint``), Symfony2 automáticamente buscará otra clase, ``MyConstraintValidator`` cuando realmente lleva a cabo la validación.

La clase "validator" también es simple, y sólo tiene un método requerido: ``isValid``.
Tomemos al ``NotBlankValidator`` como ejemplo:

.. code-block:: php

    class NotBlankValidator extends ConstraintValidator
    {
        public function isValid($value, Constraint $constraint)
        {
            if (null === $value || '' === $value) {
                $this->setMessage($constraint->message);

                return false;
            }

            return true;
        }
    }

Restricción de validadores con dependencias
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si la restricción del validador tiene dependencias, tal como una conexión de base de datos, se tendrá que configurar como un servicio en el contenedor de inyección de dependencias. Este servicio debe incluir la etiqueta ``validator.constraint_validator`` y un atributo ``alias``:

.. configuration-block::

    .. code-block:: yaml

        services:
            validator.unique.your_validator_name:
                class: Fully\Qualified\Validator\Class\Name
                tags:
                    - { name: validator.constraint_validator, alias: alias_name }

    .. code-block:: xml

        <service id="validator.unique.your_validator_name" class="Fully\Qualified\Validator\Class\Name">
            <argument type="service" id="doctrine.orm.default_entity_manager" />
            <tag name="validator.constraint_validator" alias="alias_name" />
        </service>

    .. code-block:: php

        $contenedor
            ->register('validator.unique.your_validator_name', 'Fully\Qualified\Validator\Class\Name')
            ->addTag('validator.constraint_validator', array('alias' => 'alias_name'))
        ;

Tu clase de restricción ahora podrá utilizar este alias para referirse al validador correspondiente::

    public function validatedBy()
    {
        return 'alias_name';
    }