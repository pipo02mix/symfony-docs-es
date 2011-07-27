.. index::
   single: Despachador de eventos

Cómo personalizar el comportamiento de un método sin utilizar herencia
======================================================================

Haciendo algo antes o después de llamar a un método
---------------------------------------------------

Si quieres hacer algo justo antes o justo después de invocar a un método, puedes enviar un evento, al principio o al final del método, respectivamente::

    class Foo
    {
        // ...

        public function send($foo, $bar)
        {
            // hace algo antes del método
            $evento = new FilterBeforeSendEvent($foo, $bar);
            $this->despachador->dispatch('foo.pre_send', $evento);

            // obtiene $foo y $bar desde el evento, esto se puede modificar
            $foo = $evento->getFoo();
            $bar = $evento->getBar();
            // aquí va la implementación real
            // $ret = ...;

            // hace algo después del método
            $evento = new FilterSendReturnValue($ret);
            $this->despachador->dispatch('foo.post_send', $evento);

            return $evento->getReturnValue();
        }
    }

En este ejemplo, se lanzan dos eventos: ``foo.pre_send``, antes de ejecutar el método, y ``foo.post_send`` después de ejecutar el método. Cada uno utiliza una clase Evento personalizada para comunicar información a los escuchas de los dos eventos. Estas clases de evento se tendrían que crear por ti y deben permitir que, en este ejemplo, las variables ``$foo``, ``$bar`` y ``$ret`` sean recuperadas y establecidas por los escuchas.

Por ejemplo, suponiendo que el ``FilterSendReturnValue`` tiene un método ``setReturnValue``, un escucha puede tener este aspecto:

.. code-block:: php

    public function onFooPostSend(FilterSendReturnValue $evento)
    {
        $ret = $evento->getReturnValue();
        // modifica el valor original ``$ret``

        $evento->setReturnValue($ret);
    }
