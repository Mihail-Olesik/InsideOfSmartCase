from module.module import Module
import config
import base64


class Modules:
    def initializeModules():
        for module in config.dictOfModules:
            if not config.dictOfModules[module].initializeConnection():
                return module

    def closeConnections():
        for module in config.dictOfModules:
            config.dictOfModules[module].closeConnection()

    def getData():
        data = {}
        for module in config.dictOfModules:
            value = config.dictOfModules[module].getData()
            if value != None:
                print(value)
                data[module] = base64.b64encode(value)

        return data

    def getModule(nameOfModule: str) -> Module:
        if nameOfModule in config.dictOfModules:
            return config.dictOfModules[nameOfModule]
        else:
            return None