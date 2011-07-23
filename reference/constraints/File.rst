File
====

Valida que un valor es la ruta a un archivo existente.

.. code-block:: yaml

    properties:
        filename:
            - File: ~

Opciones
--------

* ``maxSize``: El tamaño de archivo máximo permitido. Lo puedes proporcionar en bytes, kilobytes (con el sufijo "k") o megabytes (con el sufijo "M")
* ``mimeTypes``: Uno o más tipos MIME permitidos
* ``notFoundMessage``: El mensaje de error si no se encuentra el archivo
* ``notReadableMessage``: El mensaje de error si el archivo no se puede leer
* ``maxSizeMessage``: El mensaje de error si la validación ``maxSize`` falla
* ``mimeTypesMessage``: El mensaje de error si falla la validación ``mimeTypes``

Ejemplo: Validando el tamaño del archivo y el tipo MIME
-------------------------------------------------------

En este ejemplo utilizamos la restricción ``File`` para verificar que el archivo no exceda un tamaño máximo de 128 kilobytes y es un documento PDF.

.. configuration-block::

    .. code-block:: yaml

        properties:
            filename:
                - File: { maxSize: 128k, mimeTypes: [application/pdf, application/x-pdf] }

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Autor">
            <property name="filename">
                <constraint name="File">
                    <option name="maxSize">128k</option>
                    <option name="mimeTypes">
                        <value>application/pdf</value>
                        <value>application/x-pdf</value>
                    </option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\File(maxSize = "128k", mimeTypes = {
             *   "application/pdf",
             *   "application/x-pdf"
             * })
             */
            private $filename;
        }

    .. code-block:: php

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\File;

        class Autor
        {
            private $filename;

            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('filename', new File(array(
                    'maxSize' => '128k',
                    'mimeTypes' => array(
                        'application/pdf',
                        'application/x-pdf',
                    ),
                )));
            }
        }

Al validar el objeto con un archivo que no cumpla una de estas limitaciones, el validador devuelve un mensaje de error apropiado:

.. code-block:: text

    Acme\HolaBundle\Autor.filename:
        El archivo es muy grande (150 kB). El tamaño máximo permitido es de 128 kB
