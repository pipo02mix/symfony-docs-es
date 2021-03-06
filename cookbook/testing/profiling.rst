.. index::
   single: Pruebas; Generador de perfiles

Cómo utilizar el generador de perfiles en una prueba funcional
==============================================================

Es altamente recomendable que una prueba funcional sólo pruebe la respuesta. Pero si escribes pruebas funcionales que controlan los servidores en producción, posiblemente desees escribir pruebas en los datos del generador de perfiles, ya que te da una gran manera de ver diferentes cosas y hacer cumplir algunas métricas.

El :doc:`Profiler </book/internals/profiler>` de Symfony2 reúne una gran cantidad de datos para cada petición. Utiliza estos datos para comprobar el número de llamadas a la base de datos, el tiempo invertido en la plataforma, ... Pero antes de escribir aserciones, siempre verifica que el generador de perfiles realmente está disponible (está activado por omisión en el entorno de prueba —``test``)::

    class HolaControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $cliente = static::createClient();
            $impulsor = $cliente->request('GET', '/hola/Fabien');

            // Escribe algunas afirmaciones sobre Response
            // ...

            // Comprueba que el generador de perfiles esté activado
            if ($perfil = $cliente->getProfile()) {
                // comprueba el número de peticiones
                $this->assertTrue($perfil->get('db')->getQueryCount() < 10);

                // comprueba el tiempo gastado en la plataforma
                $this->assertTrue( $perfil->get('timer')->getTime() < 0.5);
            }
        }
    }

Si una prueba falla debido a los datos del generador de perfiles (demasiadas consultas a la BD, por ejemplo), posiblemente desees utilizar el Generador de perfiles Web para analizar la petición después de terminar las pruebas. Es fácil conseguirlo si incorporas el símbolo en el mensaje de error::

    $this->assertTrue(
        $perfil->get('db')->getQueryCount() < 30,
        sprintf('Checks that query count is less than 30 (token %s)', $perfil->getToken())
    );

.. caution::

     El almacén del generador de perfiles puede ser diferente en función del entorno (sobre todo si utilizas el almacén de datos SQLite, el cual es el valor configurado por omisión).

.. note::

    La información del generador de perfiles está disponible incluso si aíslas al cliente o si utilizas una capa HTTP para tus pruebas.

.. tip::

    Lee la API para incorporar :doc:`colectores de datos </cookbook/profiler/data_collector>` para aprender más acerca de tus interfaces.
