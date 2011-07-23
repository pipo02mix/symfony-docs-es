MaxLength
=========

Valida que la longitud de la cadena de un valor no es mayor que el límite dado.

+--------------------+----------------------------------------------------------------+
| Valida             | una cadena                                                     |
+--------------------+----------------------------------------------------------------+
| Opciones           | - ``limit``                                                    |
|                    | - ``message``                                                  |
|                    | - ``charset``                                                  |
+--------------------+----------------------------------------------------------------+
| Opción predefinida | ``limit``                                                      |
+--------------------+----------------------------------------------------------------+
| Clase              | :class:`Symfony\\Component\\Validator\\Constraints\\MaxLength` |
+--------------------+----------------------------------------------------------------+

Opciones
--------

*   ``limit`` (**default**, requerido) [tipo: integer] Esta es la longitud máxima de una cadena. Si se establece en 10, la cadena debe ser de no más de 10 caracteres de longitud.

*   ``message`` [tipo: string, predeterminado: ``Este valor es muy largo. Debe tener {{ limit }} caracteres o menos``] Este es el mensaje de error de validación cuando falla la validación.

Uso básico
----------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Blog:
            properties:
                summary:
                    - MaxLength: 100

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Blog">
            <property name="summary">
                <constraint name="MaxLength">
                    <value>100</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Blog.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            /**
             * @Assert\MaxLength(100)
             */
            protected $summary;
        }
