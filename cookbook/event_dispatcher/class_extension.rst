.. index::
   single: Despachador de eventos

Cómo extender una clase sin necesidad de utilizar herencia
==========================================================

Para permitir que varias clases agreguen métodos a otra, puedes definir el método mágico ``__call()`` de la clase que deseas ampliar de esta manera:

.. code-block:: php

    class Foo
    {
        // ...

        public function __call($method, $arguments)
        {
            // crea un evento llamado 'foo.method_is_not_found'
            $evento = new HandleUndefinedMethodEvent($this, $method, $arguments);
            $this->despachador->dispatch($this, 'foo.method_is_not_found', $evento);

            // ¿ningún escucha es capaz de procesar el evento? El método no existe
            if (!$evento->isProcessed()) {
                throw new \Exception(sprintf('Call to undefined method %s::%s.', get_class($this), $method));
            }

            // devuelve el escucha del valor de retorno
            return $evento->getReturnValue();
        }
    }

Este utiliza un ``HandleUndefinedMethodEvent`` especial que también se debe crear. Esta es una clase genérica que se podría reutilizar cada vez que necesites usar este modelo de extensión de la clase:

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class HandleUndefinedMethodEvent extends Event
    {
        protected $subject;
        protected $method;
        protected $arguments;
        protected $returnValue;
        protected $isProcessed = false;

        public function __construct($subject, $method, $arguments)
        {
            $this->subject = $subject;
            $this->method = $method;
            $this->arguments = $arguments;
        }

        public function getSubject()
        {
            return $this->subject;
        }

        public function getMethod()
        {
            return $this->method;
        }

        public function getArguments()
        {
            return $this->arguments;
        }

        /**
         * Fija el valor a devolver y detiene la notificación a otros escuchas
         */
        public function setReturnValue($val)
        {
            $this->returnValue = $val;
            $this->isProcessed = true;
            $this->stopPropagation();
        }

        public function getReturnValue($val)
        {
            return $this->returnValue;
        }

        public function isProcessed()
        {
            return $this->isProcessed;
        }
    }

A continuación, crea una clase que debe escuchar el evento ``foo.method_is_not_found`` y *añade* el método ``bar()``:

.. code-block:: php

    class Bar
    {
        public function onFooMethodIsNotFound(HandleUndefinedMethodEvent $evento)
        {
            // únicamente deseamos responder a las llamadas al método 'bar'
            if ('bar' != $evento->getMethod()) {
                // permite que otro escucha se preocupe del método desconocido devuelto
                return;
            }

            // el objeto subject (la instancia foo)
            $foo = $evento->getSubject();

            // los argumentos del método bar
            $arguments = $evento->getArguments();

            // Hacer alguna cosa
            // ...

            // fija el valor de retorno
            $evento->setReturnValue($someValue);
        }
    }

Por último, agrega el nuevo método ``bar`` a la clase ``Foo`` registrando una instancia de ``Bar`` con el evento ``foo.method_is_not_found``:

.. code-block:: php

    $bar = new Bar();
    $despachador->addListener('foo.method_is_not_found', $bar);
