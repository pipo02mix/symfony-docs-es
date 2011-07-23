Cómo crear la consola/línea de ordenes
======================================

Symfony2 viene con un componente Consola, que te permite crear ordenes para la línea de ordenes. Tus ordenes de consola se pueden utilizar para cualquier tarea repetitiva, como cronjobs, importaciones, u otros trabajos por lotes.

Creando una orden básica
------------------------

Para hacer que las ordenes de la consola estén disponibles automáticamente con Symfony2, crea un directorio ``Command`` dentro de tu paquete y crea un archivo PHP con el sufijo ``Command.php`` para cada orden que deses proveer. Por ejemplo, si deseas ampliar el ``AcmeDemoBundle`` (disponible en la edición estándar de Symfony) para darnos la bienvenida desde la línea de ordenes, crea ``GreetCommand.php`` y añade lo siguiente al mismo::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends ContainerAwareCommand
    {
        protected function configure()
        {
            $this
                ->setNombre('demo:greet')
                ->setDescription('Greet someone')
                ->addArgument('nombre', InputArgument::OPTIONAL, 'Who do you want to greet?')
                ->addOption('yell', null, InputOption::VALUE_NONE, 'If set, the task will yell in uppercase letters')
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $nombre = $input->getArgument('nombre');
            if ($nombre) {
                $text = 'Hola '.$nombre;
            } else {
                $text = 'Hola';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
            }

            $output->writeln($text);
        }
    }

Prueba la nueva consola de ordenes ejecutando lo siguiente

.. code-block:: bash

    app/console demo:greet Fabien

Esto imprimirá lo siguiente en la línea de ordenes:

.. code-block:: text

    Hola Fabien

También puedes utilizar a ``--yell`` para convertir todo a mayúsculas:

.. code-block:: bash

    app/console demo:greet Fabien --yell

Esto imprime::

    HOLA FABIEN

Coloreando la salida
~~~~~~~~~~~~~~~~~~~~

Cada vez que produces texto, puedes escribir el texto con etiquetas para colorear tu salida. Por ejemplo:

    // texto verde
    $output->writeln('<info>foo</info>');

    // texto amarillo
    $output->writeln('<comment>foo</comment>');

    // texto negro sobre fondo cian
    $output->writeln('<question>foo</question>');

    // texto blanco sobre fondo rojo
    $output->writeln('<error>foo</error>');

Utilizando argumentos de ordenes
--------------------------------

La parte más interesante de las ordenes son los argumentos y opciones que puedes hacer disponibles. Los argumentos son cadenas - separadas por espacios - que vienen después del nombre de la orden misma. Ellos están ordenados, y pueden ser opcionales u obligatorios. Por ejemplo, añade un argumento ``last_nombre`` opcional a la orden y haz que el argumento ``nombre`` sea obligatorio::

    $this
        // ...
        ->addArgument('nombre', InputArgument::REQUIRED, 'Who do you want to greet?')
        ->addArgument('apellido', InputArgument::OPTIONAL, 'Your last name?')
        // ...

Ahora tienes acceso a un argumento ``apellido`` en la orden::

    if ($apellido = $input->getArgument('apellido')) {
        $text .= ' '.$apellido;
    }

Ahora la orden se puede utilizar en cualquiera de las siguientes maneras:

.. code-block:: bash

    app/console demo:greet Fabien
    app/console demo:greet Fabien Potencier

Usando las opciones de la orden
-------------------------------

A diferencia de los argumentos, las opciones no están ordenadas (lo cual significa que las puedes especificar en cualquier orden) y se especifican con dos guiones (por ejemplo, ``--yell`` también puedes declarar un atajo de una letra que puedes invocar con un único guión como ``-y``). Las opciones son: *always* opcional, y se puede configurar para aceptar un valor (por ejemplo, ``dir=src``) o simplemente como una variable lógica sin valor (por ejemplo, ``yell``).

.. tip::

    También es posible hacer que una opción *opcionalmente* acepte un valor (de modo que ``--yell`` o ``yell=loud`` funcione). Las opciones también se pueden configurar para aceptar una matriz de valores.

Por ejemplo, añadir una nueva opción a la orden que se puede usar para especificar cuántas veces se debe imprimir el mensaje en una fila::

    $this
        // ...
        ->addOption('iterations', null, InputOption::VALUE_REQUIRED, 'How many times should the message be printed?', 1)

A continuación, utilízalo en la orden para imprimir el mensaje varias veces:

.. code-block:: php

    for ($i = 0; $i < $input->getOption('iterations'); $i++) {
        $output->writeln($text);
    }

Ahora, al ejecutar la tarea, si lo deseas, puedes especificar un indicador ``--iterations``:

.. code-block:: bash

    app/console demo:greet Fabien

    app/console demo:greet Fabien --iterations=5

El primer ejemplo sólo se imprimirá una vez, ya que ``iterations`` está vacía y el predeterminado es un ``1`` (el último argumento de ``addOption``). El segundo ejemplo se imprimirá cinco veces.

Recordemos que a las opciones no les preocupa su orden. Por lo tanto, cualquiera de las siguientes trabajará:

.. code-block:: bash

    app/console demo:greet Fabien --iterations=5 --yell
    app/console demo:greet Fabien --yell --iterations=5

Pidiendo información al usuario
-------------------------------

Al crear ordenes, tienes la capacidad de recopilar más información de los usuarios haciéndoles preguntas. Por ejemplo, supongamos que deseas confirmar una acción antes de llevarla a cabo realmente. Añade lo siguiente a tu orden::

    $dialog = $this->getHelperSet()->get('dialog');
    if (!$dialog->askConfirmation($output, '<question>Continue with this action?</question>', false)) {
        return;
    }

En este caso, el usuario tendrá que "Continuar con esta acción", y, a menos que responda con ``y``, la tarea se detendrá. El tercer argumento de ``askConfirmation`` es el valor predeterminado que se devuelve si el usuario no introduce algo.

También puedes hacer preguntas con más que una simple respuesta sí/no. Por ejemplo, si necesitas saber el nombre de algo, puedes hacer lo siguiente::

    $dialog = $this->getHelperSet()->get('dialog');
    $nombre = $dialog->ask($output, 'Por favor ingresa el nombre del elemento gráfico', 'foo');

Probando ordenes
----------------

Symfony2 proporciona varias herramientas para ayudarte a probar las ordenes. La más útil es la clase :class:`Symfony\\Component\\Console\\Tester\\CommandTester`. Esta utiliza clases entrada y salida especiales para facilitar la prueba sin una consola real::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            // simula el Kernel o crea uno dependiendo de tus necesidades
            $application = new Application($kernel);

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(array('command' => $command->getNombreCompleto()));

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

El método :method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay` devuelve lo que se ha exhibido durante una llamada normal de la consola.

.. tip::

    También puedes probar toda una aplicación de consola utilizando :class:`Symfony\\Component\\Console\\Tester\\ApplicationTester`.

Obteniendo servicios del contenedor de servicios
------------------------------------------------

Al usar :class:`Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand` como la clase base para la orden (en lugar del más básico :class:`Symfony\Component\Console\Command\Command`), tienes acceso al contenedor de servicios. En otras palabras, tienes acceso a cualquier servicio configurado.
Por ejemplo, fácilmente podrías extender la tarea para que sea traducible::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $nombre = $input->getArgument('nombre');
        $translator = $this->getContainer()->get('translator');
        if ($nombre) {
            $output->writeln($translator->trans('Hola %nombre%!', array('%nombre%' => $nombre)));
        } else {
            $output->writeln($translator->trans('Hola!'));
        }
    }

Llamando una orden existente
----------------------------

Si una orden depende de que se ejecute otra antes, en lugar de obligar al usuario a recordar el orden de ejecución, puedes llamarla directamente tú mismo.
Esto también es útil si deseas crear una "metaorden" que ejecute un montón de otras ordenes (por ejemplo, todas las ordenes que se deben ejecutar cuando el código del proyecto ha cambiado en los servidores de producción: vaciar la caché, generar sustitutos Doctrine2, volcar activos ``Assetic``, ...).

Llamar a una orden desde otra es sencillo::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $command = $this->getApplication()->find('demo:greet');

        $arguments = array(
            'nombre' => 'Fabien',
            '--yell' => true,
        );

        $input = new ArrayInput($arguments);
        $returnCode = $command->run($input, $output);

        // ...
    }

En primer lugar, :method:`Symfony\\Component\\Console\\Command\\Command::find` busca la orden que deseas ejecutar pasando el nombre de la orden.

Entonces, es necesario crear una nueva clase :class:`Symfony\\Component\\Console\\Input\\ArrayInput` con los argumentos y opciones que desees pasar a la orden.

Eventualmente, llamar al método ``run()`` en realidad ejecuta la orden y regresa el código devuelto por la orden (``0`` si todo va bien, cualquier otro número entero de otra manera).

.. note::

    La mayor parte del tiempo, llamar a una orden desde código que no se ejecuta en la línea de ordenes no es una buena idea por varias razones. En primer lugar, la salida de la orden se ha optimizado para la consola. Pero lo más importante, puedes pensar de una orden como si fuera un controlador; este debe utilizar el modelo para hacer algo y mostrar algún comentario al usuario. Así, en lugar de llamar una orden de la Web, reconstruye el código y mueve la lógica a una nueva clase.
