Callback
========

Llama a otros métodos durante la validación en el objeto. Estos métodos entonces pueden realizar cualquier tipo de validación y asignar errores cuando sea necesario:

.. code-block:: yaml

    Acme\DemoBundle\Entity\Autor:
        constraints:
            - Callback:
                methods:   [isAuthorValid]

Utilizando
----------

Al método "callback" se le pasa un objeto especial ``ExecutionContext``::

    use Symfony\Component\Validator\ExecutionContext;

    private $nombreDePila;

    public function isAuthorValid(ExecutionContext $context)
    {
        // de alguna manera obtiene un arreglo de "nombres ficticios"
        $fakeNames = array();

        // comprueba si el nombre realmente es ficticio
        if (in_array($this->getFirstName(), $fakeNames)) {
            $property_path = $context->getPropertyPath() . '.nombreDePila';
            $context->setPropertyPath($property_path);
            $context->addViolation('El nombre suena falso completamente', array(), null);
        }
    }

Opciones
--------

* ``methods``: Los nombres de métodos que se deben ejecutar como retrollamadas.
* ``message``: El mensaje de error si falla la validación
