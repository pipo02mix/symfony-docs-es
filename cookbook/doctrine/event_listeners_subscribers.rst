.. _doctrine-event-config:

Registrando escuchas y suscriptores de eventos
==============================================

Doctrine utiliza la clase ligera ``Doctrine\Common\EventManager`` para desencadenar una serie de diferentes eventos que puedes enganchar. Puedes registrar escuchas de eventos o de suscriptores etiquetando los respectivos servicios ``doctrine.event_listener`` o ``doctrine.event_subscriber`` utilizando el contenedor de servicios.

Para registrar servicios para que actúen como escuchas de eventos o suscriptores (escuchas a partir de aquí) hay que etiquetarlos con los nombres adecuados. Dependiendo de tu caso de uso, puedes conectar un escucha en cada conexión DBAL y Administrador de Entidad ORM o sólo en uno específico a la conexión DBAL y todos los ``EntityManagers`` que utilizan esta conexión.

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection: default
                connections:
                    default:
                        driver: pdo_sqlite
                        memory: true

        services:
            my.listener:
                class: MyEventListener
                tags:
                    - { name: doctrine.event_listener, event: postLoad }
            my.listener2:
                class: MyEventListener2
                tags:
                    - { name: doctrine.event_listener, event: postLoad, connection: default }
            my.subscriber:
                class: MyEventSubscriber
                tags:
                    - { name: doctrine.event_subscriber, connection: default }

    .. code-block:: xml

        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection driver="pdo_sqlite" memory="true" />
                </doctrine:dbal>
            </doctrine:config>

            <services>
                <service id="my.listener" class="MyEventListener">
                    <tag name="doctrine.event_listener" event="postLoad" />
                </service>
                <service id="my.listener2" class="MyEventListener2">
                    <tag name="doctrine.event_listener" event="postLoad" connection="default" />
                </service>
                <service id="my.subscriber" class="MyEventSubscriber">
                    <tag name="doctrine.event_subscriber" connection="default" />
                </service>
            </services>
        </container>
