Cómo implementar un sencillo formulario de registro con ``MongoDB``
===================================================================

Algunos formularios tienen campos adicionales cuyos valores no es necesario almacenar en la base de datos. En este ejemplo, vamos a crear un formulario de registro con algunos campos adicionales, y ("términos aceptados" como un campo de casilla de verificación) y lo incluiremos en el formulario que realmente almacena la información de la cuenta. Vamos a utilizar ``MongoDB`` para almacenar los datos.

.. tip::

    Si no estás familiarizado con la biblioteca ``MongoDB`` de Doctrine, primero lee el artículo ":doc:`/cookbook/doctrine/mongodb`" en el recetario para aprender a configurar y trabajar con ``MongoDB`` en Symfony.

El modelo de Usuario simple
---------------------------

Por lo tanto, en esta guía comenzaremos con el modelo para un documento ``Usuario``::

    // src/Acme/AccountBundle/Document/Usuario.php
    namespace Acme\AccountBundle\Document;

    use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;
    use Symfony\Component\Validator\Constraints as Assert;
    use Symfony\Bundle\DoctrineMongoDBBundle\Validator\Constraints\Unique as MongoDBUnique;

    /**
     * @MongoDB\Document(collection="users")
     * @MongoDBUnique(path="correo")
     */
    class Usuario
    {
        /**
         * @MongoDB\Id
         */
        protected $id;

        /**
         * @MongoDB\Field(type="string")
         * @Assert\NotBlank()
         * @Assert\Correo()
         */
        protected $correo;

        /**
         * @MongoDB\Field(type="string")
         * @Assert\NotBlank()
         */
        protected $pase;

        public function getId()
        {
            return $this->id;
        }

        public function getEmail()
        {
            return $this->email;
        }

        public function setEmail($correo)
        {
            $this->email = $correo;
        }

        public function getPassword()
        {
            return $this->password;
        }

        // cifrado estúpidamente simple (¡por favor no copies esto!)
        public function setPassword($pase)
        {
            $this->password = sha1($pase);
        }
    }

Este documento ``Usuario`` contiene tres campos y dos de ellos (correo electrónico y contraseña) se deben mostrar en el formulario. La propiedad correo electrónico debe ser única en la base de datos, por lo tanto añadimos esta validación en lo alto de la clase.

.. note::

    Si deseas integrar este Usuario en el sistema de seguridad, es necesario implementar la :ref:`Interfaz de usuario <book-security-user-entity>` del componente Seguridad.

Creando un formulario para el modelo
------------------------------------

A continuación, crea el formulario para el modelo ``Usuario``:

    // src/Acme/AccountBundle/Form/Type/UserType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\Extension\Core\Type\RepeatedType;
    use Symfony\Component\Form\FormBuilder;

    class UserType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $opciones)
        {
            $builder->add('email', 'correo');
            $builder->add('password', 'repeated', array(
               'first_name' => 'password',
               'second_name' => 'confirm',
               'type' => 'password'
            ));
        }

        public function getDefaultOptions(array $opciones)
        {
            return array('data_class' => 'Acme\AccountBundle\Document\User');
        }

        public function getNombre()
        {
            return 'user';
        }
    }

Acabamos de añadir dos campos: correo electrónico y contraseña (repetido para confirmar la contraseña introducida). La opción ``data_class`` le indica al formulario el nombre de la clase de datos (es decir, el documento ``Usuario``).

.. tip::

    Para explorar más cosas sobre el componente Formulario, lee esta documentación :doc:`archivo </book/forms>`.

Incorporando el formulario Usuario en un formulario de inscripción
------------------------------------------------------------------

El formulario que vamos a usar para la página de registro no es el mismo que el formulario utilizado para simplemente modificar al ``Usuario`` (es decir, ``UserType``). El formulario de registro contiene más campos como "acepto las condiciones", cuyo valor no se almacenará en la base de datos.

En otras palabras, creas un segundo formulario de inscripción, el cual incorpora el formulario ``Usuario`` y añade el campo extra necesario. Empecemos creando una clase simple que representa el "registro"::

    // src/Acme/AccountBundle/Form/Model/Alta.php
    namespace Acme\AccountBundle\Form\Model;

    use Symfony\Component\Validator\Constraints as Assert;

    use Acme\AccountBundle\Document\User;

    class Alta
    {
        /**
         * @Assert\Type(type="Acme\AccountBundle\Document\User")
         */
        protected $user;

        /**
         * @Assert\NotBlank()
         * @Assert\True()
         */
        protected $termsAccepted;

        public function setUser(User $user)
        {
            $this->user = $user;
        }

        public function getUser()
        {
            return $this->user;
        }

        public function getTermsAccepted()
        {
            return $this->termsAccepted;
        }

        public function setTermsAccepted($termsAccepted)
        {
            $this->termsAccepted = (boolean)$termsAccepted;
        }
    }

A continuación, crea el formulario para el modelo ``Registro``:

    // src/Acme/AccountBundle/Form/Type/RegistrationType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\Extension\Core\Type\RepeatedType;
    use Symfony\Component\Form\FormBuilder;

    class RegistrationType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $opciones)
        {
            $builder->add('user', new UserType());
            $builder->add('terms', 'checkbox', array('property_path' => 'termsAccepted'));
        }

        public function getNombre()
        {
            return 'alta';
        }
    }

No necesitas utilizar métodos especiales para incrustar el formulario ``UserType``.
Un formulario es un campo, también - por lo tanto lo puedes añadir como cualquier otro campo, con la expectativa de que la propiedad ``Usuario`` correspondiente mantendrá una instancia de la clase ``UserType``.

Manejando el envío del formulario
---------------------------------

A continuación, necesitas un controlador para manejar el formulario. Comienza creando un controlador simple para mostrar el formulario de inscripción::

    // src/Acme/AccountBundle/Controller/AccountController.php
    namespace Acme\AccountBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    use Acme\AccountBundle\Form\Type\RegistrationType;
    use Acme\AccountBundle\Form\Model\Alta;

    class AccountController extends Controller
    {
        public function registerAction()
        {
            $formulario = $this->createForm(new RegistrationType(), new Alta());

            return $this->render('AcmeAccountBundle:Account:register.html.twig', array('form' => $formulario->createView()));
        }
    }

y su plantilla:

.. code-block:: html+jinja

    {# src/Acme/AccountBundle/Resources/views/Account/register.html.twig #}

    <form action="{{ path('create')}}" method="post" {{ form_enctype(form) }}>
        {{ form_widget(form) }}

        <input type="submit" />
    </form>

Finalmente, crea el controlador que maneja el envío del formulario.  Esto realiza la validación y guarda los datos en ``MongoDB``::

    public function createAction()
    {
        $dm = $this->get('doctrine.odm.mongodb.default_document_manager');

        $formulario = $this->createForm(new RegistrationType(), new Alta());

        $formulario->bindRequest($this->getRequest());

        if ($formulario->isValid()) {
            $registration = $formulario->getData();

            $dm->persist($registration->getUser());
            $dm->flush();

            return $this->redirect(...);
        }

        return $this->render('AcmeAccountBundle:Account:register.html.twig', array('form' => $formulario->createView()));
    }

¡Eso es todo! Tu formulario ahora valida, y te permite guardar el objeto ``Usuario`` a MongoDB.
