MinLength
=========

Valida que la longitud de la cadena de un valor no es menor que el límite dado.

+--------------------+----------------------------------------------------------------+
| Valida             | una cadena                                                     |
+--------------------+----------------------------------------------------------------+
| Opciones           | - ``limit``                                                    |
|                    | - ``message``                                                  |
|                    | - ``charset``                                                  |
+--------------------+----------------------------------------------------------------+
| Opción predefinida | ``limit``                                                      |
+--------------------+----------------------------------------------------------------+
| Clase              | :class:`Symfony\\Component\\Validator\\Constraints\\MinLength` |
+--------------------+----------------------------------------------------------------+

Opciones
--------

*   ``limit`` (**default**, requerido) [tipo: integer] Esta es la longitud mínima de una cadena. Si se establece en 3, la cadena debe ser de al menos 3 caracteres de longitud.

*   ``message`` [tipo: string, predeterminado: ``Este valor es muy corto. Debe tener por lo menos {{ limit }} caracteres o más``] Este es el mensaje de error mostrado cuando la validación falla.

Uso básico
----------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Autor:
            properties:
                nombreDePila:
                    - MinLength: 3

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Autor">
            <property name="nombreDePila">
                <constraint name="MinLength">
                    <value>3</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\MinLength(3)
             */
            protected $nombreDePila;
        }
