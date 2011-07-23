Cómo manejar con Doctrine los archivos subidos
==============================================

Manejar el envío de archivos con entidades Doctrine no es diferente a la manipulación de cualquier otra carga de archivo. En otras palabras, eres libre de mover el archivo en tu controlador después de manipular el envío de un formulario. Para ver ejemplos de cómo hacerlo, consulta el :doc:`/reference/forms/types/file` en la referencia.

Si lo deseas, también puedes integrar la carga de archivos en el ciclo de vida de tu entidad (es decir, creación, actualización y eliminación). En este caso, ya que tu entidad es creada, actualizada y eliminada desde Doctrine, el proceso de carga y remoción de archivos se llevará a cabo de forma automática (sin necesidad de hacer nada en el controlador);

Para que esto funcione, tendrás que hacerte cargo de una serie de detalles, los cuales serán cubiertos en este artículo del recetario.

Configuración básica
--------------------

En primer lugar, crea una sencilla clase Entidad de Doctrine con la cual trabajar::

    // src/Acme/DemoBundle/Entity/Document.php
    namespace Acme\DemoBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Validator\Constraints as Assert;

    /**
     * @ORM\Entity
     */
    class Document
    {
        /**
         * @ORM\Id @ORM\Column(type="integer")
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        public $id;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank
         */
        public $nombre;

        /**
         * @ORM\Column(type="string", length=255, nullable=true)
         */
        public $ruta;

        public function getFullPath()
        {
            return null === $this->path ? null : $this->getUploadRootDir().'/'.$this->path;
        }

        protected function getUploadRootDir()
        {
            // la ruta absoluta al directorio dónde se deben guardar los archivos cargados
            return __DIR__.'/../../../../web/uploads/documents';
        }
    }

La entidad ``Documento`` tiene un nombre y está asociado con un archivo. La propiedad ``path`` almacena la ruta relativa al archivo y se persiste en la base de datos.
``getFullPath()`` es un método útil que utiliza el método ``getUploadRootDir()`` para devolver la ruta absoluta al archivo.

.. tip::

    Si no lo haz hecho, probablemente primero deberías leer el tipo :doc:`archivo </reference/forms/types/file>` en la documentación para comprender cómo trabaja el proceso de carga básico.

.. note::

    Si estás utilizando anotaciones para especificar tus reglas de anotación (como muestra este ejemplo), asegúrate de que haz habilitado la validación por medio de anotaciones (consulta :ref:`configurando la validación <book-validation-configuration>`).

Para manejar el archivo subido real en el formulario, utiliza un campo ``file`` "virtual".
Por ejemplo, si estás construyendo tu formulario directamente en un controlador, podría tener este aspecto::

    public function uploadAction()
    {
        // ...

        $formulario = $this->createFormBuilder($document)
            ->add('nombre')
            ->add('file')
            ->getForm()
        ;

        // ...
    }

A continuación, crea esta propiedad en tu clase ``Documento`` y agrega algunas reglas de validación::

    // src/Acme/DemoBundle/Entity/Document.php

    // ...
    class Document
    {
        /**
         * @Assert\File(maxSize="6000000")
         */
        public $file;

        // ...
    }

.. note::

    Debido a que estás utilizando la restricción ``File``, Symfony2 automáticamente supone que el campo del formulario es una entrada de carga archivo. Es por eso que no lo tienes que establecer explícitamente al crear el formulario anterior (``->add('file')``).

El siguiente controlador muestra cómo manipular todo el proceso::

    use Acme\DemoBundle\Entity\Document;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    // ...

    /**
     * @Template()
     */
    public function uploadAction()
    {
        $document = new Document();
        $formulario = $this->createFormBuilder($document)
            ->add('nombre')
            ->add('file')
            ->getForm()
        ;

        if ($this->getRequest()->getMethod() === 'POST') {
            $formulario->bindRequest($this->getRequest());
            if ($formulario->isValid()) {
                $em = $this->getDoctrine()->getEntityManager();

                $em->persist($document);
                $em->flush();

                $this->redirect($this->generateUrl('...'));
            }
        }

        return array('form' => $formulario->createView());
    }

.. note::

    Al escribir la plantilla, no olvides fijar el atributo ``enctype``:

    .. code-block:: html+php

        <h1>Subir Archivo</h1>

        <form action="#" method="post" {{ form_enctype(form) }}>
            {{ form_widget(form) }}

            <input type="submit" value="Cargar Documento" />
        </form>

El controlador anterior automáticamente persistirá la entidad ``Documento`` con el nombre presentado, pero no hará nada sobre el archivo y la propiedad ``path`` quedará en blanco.

Una manera fácil de manejar la carga de archivos es que lo muevas justo antes de que se persista la entidad y a continuación, establece la propiedad ``path`` en consecuencia. Comienza por invocar a un nuevo método ``upload()`` en la clase ``Documento``, el cual deberás crear en un momento para manejar la carga del archivo::

    if ($formulario->isValid()) {
        $em = $this->getDoctrine()->getEntityManager();

        $document->upload();

        $em->persist($document);
        $em->flush();

        $this->redirect('...');
    }

El método ``upload()`` tomará ventaja del objeto :class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`, el cual es lo que devuelve después de que se presenta un campo ``file``::

    public function upload()
    {
        // la propiedad 'file' puede estar vacía si el campo no es obligatorio
        if (null === $this->file) {
            return;
        }

        // aquí utilizamos el nombre de archivo original pero lo deberías
        // desinfectar por lo menos para evitar cualquier problema de seguridad

        // 'move' toma el directorio y nombre de archivo destino al cual trasladarlo
        $this->file->move($this->getUploadRootDir(), $this->file->getClientOriginalName());

        // fija la propiedad 'path' al nombre de archivo donde se guardó el archivo
        $this->setPath($this->file->getClientOriginalName());

        // limpia la propiedad 'file' puesto que ya no la vas a necesitar
        unset($this->file);
    }

Usando el ciclo de vida de las retrollamadas
--------------------------------------------

Incluso si esta implementación trabaja, adolece de un defecto importante: ¿Qué pasa si hay un problema al persistir la entidad? El archivo ya se ha movido a su ubicación final, incluso aunque la propiedad ``path`` de la entidad no se persista correctamente.

Para evitar estos problemas, debes cambiar la implementación para que la operación de base de datos y el traslado del archivo sean atómicos: si hay un problema al persistir la entidad o si el archivo no se puede mover, entonces, no debe suceder *nada*.

Para ello, es necesario mover el archivo justo cuando Doctrine persista la entidad a la base de datos. Esto se puede lograr enganchando el ciclo de vida de la entidad a una retrollamada::

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
    }

A continuación, reconstruye la clase ``Documento`` para que tome ventaja de estas retrollamadas::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                // haz lo que quieras para generar un nombre único
                $this->setPath(uniq().'.'.$this->file->guessExtension());
            }
        }

        /**
         * @ORM\PostPersist()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // aquí debes lanzar una excepción si el archivo no se puede mover
            // para que la entidad no se persista en la base de datos
            // lo cual hace automáticamente el método move() del archivo subido
            $this->file->move($this->getUploadRootDir(), $this->path);

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getFullPath()) {
                unlink($file);
            }
        }
    }

La clase ahora hace todo lo que necesitas: genera un nombre de archivo único antes de persistirlo, mueve el archivo después de persistirlo y elimina el archivo si la entidad es eliminada.

Usando el ``id`` como nombre de archivo
---------------------------------------

Si deseas utilizar el ``id`` como el nombre del archivo, la implementación es un poco diferente conforme sea necesaria para guardar la extensión en la propiedad ``path``, en lugar del nombre de archivo real::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                $this->setPath($this->file->guessExtension());
            }
        }

        /**
         * @ORM\PostPersist()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // aquí debes lanzar una excepción si el archivo no se puede mover
            // para que la entidad no se conserve en la base de datos
            // lo cual hace el método move() del archivo subido
            $this->file->move($this->getUploadRootDir(), $this->id.'.'.$this->file->guessExtension());

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getFullPath()) {
                unlink($file);
            }
        }

        public function getFullPath()
        {
            return null === $this->path ? null : $this->getUploadRootDir().'/'.$this->id.'.'.$this->path;
        }
    }
