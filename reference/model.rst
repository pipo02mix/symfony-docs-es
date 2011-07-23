.. index::
   single: Modelo

Introducción al "Modelo"
========================

Si quieres aprender más sobre las modelos de la moda y las supermodelos, entonces esta sección no te será de utilidad. Pero si estás buscando aprender sobre el modelo - la capa de la aplicación que gestiona los datos - entonces sigue leyendo.
La descripción del modelo en esta sección es la que se utiliza cuando hablamos de aplicaciones *Modelo-Vista-Controlador*.

.. note::

   Modelo-Vista-Controlador (MVC) es un patrón de diseño de la aplicación, introducido originalmente por Trygve Reenskaug para la plataforma Smalltalk. La idea principal del MVC es separar la presentación de los datos y separar el controlador de la presentación. Este tipo de separación permite a cada parte de la aplicación enfocarse en exactamente un objetivo. El controlador se enfoca en cambiar los datos del modelo, el Modelo expone sus datos para la vista y la vista se centra en la creación de las representaciones del modelo (por ejemplo una página HTML que muestra "los mensajes del blog").

Por ejemplo, cuando un usuario accede a la página principal de tu blog, el navegador del usuario envía una petición, que se transmite al controlador responsable de la presentación de los mensajes. El controlador calcula cuales mensajes se deben mostrar, recupera ``Post`` de los Modelos de la base de datos y pasa la matriz a la vista. La vista reproduce el HTML que es interpretado por el navegador.

¿De cualquier manera, qué es un modelo?
---------------------------------------

El *modelo* es lo que significa la "M" en `MVC`_. Es uno de los tres pilares de una aplicación MVC. Un modelo es responsable de cambiar su estado interno basándose en las peticiones del :doc:`controlador
</quick_tour/the_controller>` y dar su información de estado actual a la :doc:`vista </book/templating>`. Este es el contenedor lógico de la aplicación principal.

Por ejemplo, si estás construyendo un blog, entonces tendrás un modelo ``Post``. Si estás construyendo un sistema gestor de contenido, entonces necesitas un modelo ``Página``.

.. code-block:: php

    <?php

    namespace Blog;

    class Post
    {
        private $titulo;
        private $cuerpo;
        private $createdAt;
        private $updatedAt;

        public function __construct($titulo, $cuerpo)
        {
            $this->titulo     = $titulo;
            $this->cuerpo      = $cuerpo;
            $this->createdAt = new \DateTime();
        }

        public function setTitle($titulo)
        {
            $this->titulo     = $titulo;
            $this->updatedAt = new \DateTime();
        }

        public function setCuerpo($cuerpo)
        {
            $this->body      = $cuerpo;
            $this->updatedAt = new \DateTime();
        }

        public function getTitulo()
        {
            return $this->titulo;
        }

        public function getCuerpo()
        {
            return $this->cuerpo;
        }
    }

Es obvio que la clase anterior es muy simple y comprobable, sin embargo, casi está completa, y cumplirá todas las necesidades de un motor de 'blogs' simple.

¡Eso es todo! Ahora ya sabes lo que es un modelo en Symfony2: se trata de cualquier clase que deses guardar en una especie de mecanismo de almacenamiento y posterior recuperación de datos. El resto del capítulo está dedicado a explicar cómo interactuar con la base de datos.

Bases de datos y Symfony2
-------------------------

Cabe señalar que Symfony2 no viene con un asignador objeto↔relacional (ORM) o capa de abstracción de bases de datos (DBAL) propia; el cual no sólo es el problema que Symfony2 intenta resolver. Sin embargo, Symfony2 ofrece una profunda integración con bibliotecas como `Doctrine`_ y `Propel`_, que *proporcionan* paquetes ORM y DBAL, lo cual te permite usar el que más te guste.

.. note::

   El acrónimo "ORM" significa "Object Relational Mapping" y representa una técnica de programación consistente en la conversión de datos entre sistemas de tipos incompatibles. Digamos que tenemos un ``Post``, que se almacena como un conjunto de columnas en una base de datos, pero representada por una instancia de la clase ``Post`` en tu aplicación. Al proceso de transformar de una tabla de base de datos a un objeto se le conoce cómo *asignación de relación a objeto*.
   También veremos que este término es un poco anticuado, ya que se utiliza en el tratamiento de sistemas de gestión de base de datos relacionales. Hoy día existen muchos mecanismos de almacenamiento de datos no relacionales. Uno de esos mecanismos es la *base de datos orientada a documento* (por ejemplo, MongoDB), la cual utiliza un nuevo término, "Asignación de objeto a documento" u "ODM".

En breve, aprenderás acerca del `ORM de Doctrine2`_ y `Doctrine2 MongoDB ODM`_ (que sirve como ODM para MongoDB_ - un almacén de documentos popular) ya que ambos tienen la más profunda integración con Symfony2 al momento de escribir este artículo.

Un modelo no es una tabla
-------------------------

La percepción de una clase modelo como una tabla de base de datos, donde cada instancia de objeto representa una sola fila, fue popularizada por la plataforma Ruby on Rails y el patrón de diseño `Active Record`_. Esta es una buena manera de pensar primero en la capa del modelo de tu aplicación, especialmente si estás exponiendo una simple interfaz `CRUD`_ (Crear, Recuperar, Actualizar - 'Update', Eliminar - 'Delete') para modificar los datos de un modelo.

Pero este enfoque realmente puede causar problemas una vez que estés más allá de la parte 'CRUD' de tu aplicación y empieces a añadir más lógica del negocio. Aquí están las limitaciones comunes del enfoque descrito anteriormente:

* Diseñar un esquema antes que el software real que lo utilizará es como cavar un agujero antes de saber lo que hay que enterrar.
  El elemento podría encajar en el agujero excavado, ¿pero si lo que estás enterrando es un gran camión de bomberos? Se requiere un enfoque totalmente diferente si quieres hacer eficientemente el trabajo.

* Una base de datos se debe adaptar a las necesidades de tu aplicación, no a la inversa.

* Algunos motores de almacenamiento de datos (como las bases de datos documentales) no tienen una noción de tablas, filas o incluso un esquema, por lo que son difíciles de usar si tu percepción de un modelo es la que representa una tabla.

* Mantener en mente el esquema de la base de datos mientras diseñas el dominio de tu aplicación es problemático, y seguir la regla del mínimo común denominador te dará lo peor de ambos mundos.

El `ORM de Doctrine2`_ está diseñado para eliminar la necesidad de mantener la estructura de la base de datos en mente y te permite concentrarte en la escritura de los modelos más limpios posibles que satisfagan tus necesidades. Esto te permite diseñar tus clases e interacciones en primer lugar, antes de exigirte pensar en *cómo* persistir tus datos.

Cambiando el paradigma
----------------------

Con la introducción de Doctrine2, algunos de los principales paradigmas han cambiado. El `Diseño Dirigido por Dominio`_ (Domain Driven Design -DDD) nos enseña que los objetos se modelan mejor después de su prototipo en el mundo real. Por ejemplo, un objeto ``Coche`` es el mejor modelo para contener ``Motor``, cuatro instancias de ``Neumáticos``, etc. y se debe producir por ``FábricaDeCoches`` - algo que sabe cómo ensamblar todas las partes. El diseño dirigido por el dominio merece un libro en sí mismo, ya que el concepto es más amplio. Sin embargo, a efectos de este capítulo, debe quedar claro que un coche no se puede arrancar por sí mismo, debe haber un impulso externo para ello. De manera similar, un modelo no puede salvarse a sí mismo sin un impulso externo, por lo que el siguiente fragmento de código viola el DDD y debe ser molesto rediseñarlo de manera limpia, comprobable.

.. code-block:: php

   $mensaje->save();

Por lo tanto, Doctrine2 no es más tu típica implementación `Active Record`_.
En su lugar Doctrine2 utiliza un conjunto de patrones diferente, los patrones más importantes `Asignador de datos`_ y `Unidad de trabajo`_. El siguiente ejemplo muestra cómo salvar una entidad con Doctrine2:

.. code-block:: php

   $manager = //... obtiene una instancia del administrador de objetos

   $manager->persist($mensaje);
   $manager->flush();

El "administrador de objetos" es un objeto central provisto por Doctrine cuyo trabajo es persistir objetos. Pronto aprenderás mucho más sobre este servicio.
Este cambio de paradigma nos permite deshacernos de las clases base (por ejemplo, ``Mensaje`` no necesita extender una clase base) y las dependencias estáticas. Cualquier objeto se puede guardar en una base de datos para su posterior recuperación. Más que eso, una vez persistido, un objeto es gestionado por el administrador de objetos hasta que se elimina el administrador explícitamente. Esto significa que todas las interacciones de objeto ocurren en memoria, sin tener que ir a la base de datos hasta que se llama a ``$manager->flush()``. Huelga decir que esto proporciona una base de datos e instantánea optimización de consultas comparado con la mayoría de los patrones de persistencia, puesto que todas las consultas son diferidas lo más posible (es decir, su ejecución se aplaza hasta el último momento posible).

Un aspecto muy importante del patrón `Active Record`_ es el rendimiento, o más bien, la *dificultad* en la construcción de un sistema funcionando. Al usar las transacciones y el objeto en memoria cambia el control, Doctrine2 minimiza la comunicación con la base de datos, ahorrando no sólo el tiempo de ejecución de la base de datos, sino también costosa comunicación de red.

Conclusión
----------

Gracias a Doctrine2, el modelo ahora probablemente sea el concepto más sencillo de Symfony2: este tiene el control total y no limitado por la persistencia específica.

Al asociarlo con Doctrine2 para mantener el código liberado de los detalles de la  persistencia, hace aún más simple la construcción de aplicaciones con bases de datos. El código de la aplicación se mantiene limpio, lo cual reduce el tiempo de desarrollo y mejora la comprensibilidad del código.

.. _Doctrine: http://www.doctrine-project.org/
.. _Propel: http://www.propelorm.org/
.. _`DBAL de Doctrine2`: http://www.doctrine-project.org/projects/dbal
.. _ORM de Doctrine2: http://www.doctrine-project.org/projects/orm
.. _MongoDB ODM: http://www.doctrine-project.org/projects/mongodb_odm
.. _MongoDB: http://www.mongodb.org
.. _`Diseño Dirigido por Dominio`: http://domaindrivendesign.org/
.. _`Active Record`: http://martinfowler.com/eaaCatalog/activeRecord.html
.. _`Asignador de datos`: http://martinfowler.com/eaaCatalog/dataMapper.html
.. _`Unidad de trabajo`: http://martinfowler.com/eaaCatalog/unitOfWork.html
.. _CRUD: http://es.wikipedia.org/wiki/CRUD
.. _MVC: http://es.wikipedia.org/wiki/Modelo_Vista_Controlador