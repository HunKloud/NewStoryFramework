class INewStoryGroupRPC
{
    // Добавляет RPC:
    //  * key                               - основной ключ, например название мода
    //  * type                              - по сути аналог rpc_type, или же индентификатор для callback
    //  * callback (не может иметь NULL)    - callback функция, куда будем перенаправлять полученные данные,
    //                                          кроме того callback функция должна иметь 2 входных параметра:
    //                                          (PlayerIdentity sender, ParamsReadContext ctx)
    //  * instance_type (необязателен)      - где именно надо зарегестрировать вызов:
    //                                          ENewStoryGroupRPCInstance.CLIENT - на клиенте
    //                                          ENewStoryGroupRPCInstance.SERVER - на сервере
    //                                          ENewStoryGroupRPCInstance.ANY    - и на клиенте, и на сервере (базовое значение)
    // Example:
    // void TestAddRPC()
    // {
    //     string  your_rpc_key    = "NSGFramework";
    //     int     your_rpc_type   = 0;
    //     NewStoryGroup().RPC().Add( your_rpc_key, your_rpc_type, ScriptCaller.Create( your_fnc ) );
    // }
    void Add(string key, int type, notnull ScriptCaller callback, int instance_type = ENewStoryGroupRPCInstance.ANY);

    // Удаляет RPC:
    //  * key                               - ключ под которым зарегестрирован callback
    //  * type                              - идентификатор callback
    //  * instance_type (необязателен)      - где именно удаляем зарегестрированный вызов:
    //                                          ENewStoryGroupRPCInstance.CLIENT - на клиенте
    //                                          ENewStoryGroupRPCInstance.SERVER - на сервере
    //                                          ENewStoryGroupRPCInstance.ANY    - и на клиенте, и на сервере (базовое значение)
    void Remove(string key, int type, int instance_type = ENewStoryGroupRPCInstance.ANY);

    // Отправляет список данных:
    //  * key                               - ключ под которым зарегестрирован callback,
    //                                          если вызывается на стороне сервера, то указываем название ключа зарегестрированного на клиенте и наоборот
    //  * type                              - идентификатор под которым зарегестрирован callback, аналогично с key
    //  * params                            - список данных
    //  * recipient                         - идентификатор получателя, если указано NULL, то всем игрокам, если на стороне сервера,
    //                                          на клиенте не обязателен?...
	void Send(string key, int type, array<ref Param> params, PlayerIdentity recipient = null);

    // Анологично как и 'Send', только для списка получателей
    void SendToPlayers(string key, int type, array<ref Param> params, notnull array<PlayerIdentity> recipients);

    // Отправляет данные:
    //  * key                               - ключ под которым зарегестрирован callback,
    //                                          если вызывается на стороне сервера, то указываем название ключа зарегестрированного на клиенте и наоборот
    //  * type                              - идентификатор под которым зарегестрирован callback, аналогично с key
    //  * params                            - данные
    //  * recipient                         - идентификатор получателя, если указано NULL, то всем игрокам, если на стороне сервера,
    //                                          на клиенте не обязателен?...
	void SendSingleParam(string key, int type, Param param, PlayerIdentity recipient = null);

    // Анологично как и 'SendSingleParam', только для списка получателей
    void SendSingleParamToPlayers(string key, int type, Param param, notnull array<PlayerIdentity> recipients);
}
