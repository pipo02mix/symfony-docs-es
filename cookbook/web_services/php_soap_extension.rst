.. index::
    single: Servicios Web; SOAP

Cómo crear un servicio Web SOAP en un controlador de Symfony2
=============================================================

Configurar un controlador para que actúe como un servidor SOAP es muy sencillo con un par de herramientas.  Debes, por supuesto, tener instalada la extensión `SOAP de PHP`_.  
Debido a que la extensión SOAP de PHP, actualmente no puede generar un WSDL, debes crear uno desde cero o utilizar un generador de un tercero.

A continuación mostramos un ejemplo de un controlador que es capaz de manejar una petición SOAP.  Si ``indexAction()`` es accesible a través de la ruta ``/soap``, se puede recuperar el documento WSDL a través de ``/soap?wsdl``.

.. code-block:: php

    class MiSoapController extends Controller 
    {
        public function indexAction()
        {
            $servidor = new \SoapServer('/ruta/a/hola.wsdl');

            $servidor->setObjeto($this);

            $respuesta = new Response();

            $respuesta->headers->set('Content-Type', 'text/xml; charset=ISO-8859-1');

            ob_start();

            $servidor->handle();

            $respuesta->setContent(ob_get_clean());

            return $respuesta;
        }
 
        public function hola($nombre)
        {
            return 'Hola, ' . $nombre . '!';
        }
    }

Toma nota de las llamadas a ``ob_start()`` y ``ob_get_clean()``.  Estos métodos controlan la `salida almacenada temporalmente`_ que te permite "atrapar" la salida difundida por el ``$servidor->handle()``. Esto es necesario porque Symfony espera que el controlador devuelva un objeto ``Respuesta`` con la salida como "contenido".
También debes recordar establecer la cabecera ``Content-Type`` a ``text/xml``, ya que esto es lo que espera el cliente.  Por lo tanto, utiliza ``ob_start()`` para empezar a amortiguar la ``STDOUT`` y usa ``ob_get_clean()`` para volcar la salida difundida al contenido de la ``Respuesta`` y limpiar la salida.  Por último, estás listo para devolver la ``Respuesta``.

A continuación hay un ejemplo de llamada al servicio usando el cliente `NuSOAP`_.  Este ejemplo asume que el ``indexAction`` del controlador de arriba es accesible a través de la ruta ``/soap``::

    $cliente = new soapclient('http://ejemplo.com/app.php/soap?wsdl', true);

    $resultado = $cliente->call('hola', array('nombre' => 'Scott'));

Abajo está un ejemplo de WSDL.

.. code-block:: xml

    <?xml version="1.0" encoding="ISO-8859-1"?>
     <definitions xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" 
         xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
         xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/" 
         xmlns:tns="urn:arnleadservicewsdl" 
         xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/" 
         xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/" 
         xmlns="http://schemas.xmlsoap.org/wsdl/" 
         targetNamespace="urn:arnleadservicewsdl">
      <types>
       <xsd:schema targetNamespace="urn:holawsdl">
        <xsd:import namespace="http://schemas.xmlsoap.org/soap/encoding/" />
        <xsd:import namespace="http://schemas.xmlsoap.org/wsdl/" />
       </xsd:schema>
      </types>
      <message name="holaRequest">
       <part name="name" type="xsd:string" />
      </message>
      <message name="holaResponse">
       <part name="return" type="xsd:string" />
      </message>
      <portType name="holawsdlPortType">
       <operation name="hello">
        <documentation>Hola Mundo</documentation>
        <input message="tns:holaRequest"/>
        <output message="tns:holaResponse"/>
       </operation>
      </portType>
      <binding name="holawsdlBinding" type="tns:holawsdlPortType">
      <soap:binding style="rpc" transport="http://schemas.xmlsoap.org/soap/http"/>
      <operation name="hola">
       <soap:operation soapAction="urn:arnleadservicewsdl#hola" style="rpc"/>
       <input>
        <soap:body use="encoded" namespace="urn:holawsdl" 
            encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
       </input>
       <output>
        <soap:body use="encoded" namespace="urn:holawsdl" 
            encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
       </output>
      </operation>
     </binding>
     <service name="holawsdl">
      <port name="holawsdlPort" binding="tns:holawsdlBinding">
       <soap:domicilio location="http://ejemplo.com/app.php/soap" />
      </port>
     </service>
    </definitions>


.. _`SOAP de PHP`:          http://php.net/manual/en/book.soap.php
.. _`NuSOAP`:            http://sourceforge.net/projects/nusoap
.. _`salida almacenada temporalmente`:  http://www.php.net/manual/es/book.outcontrol.php
