def operacoes(func):
    def wrapper(*args):
        args[0].extrato.append(func(*args))
        print("\033[1;32mOperação Executada!\033[1;m")
        
    return wrapper
        
    
def loggin(func):
    def wrapper(*args):
        if args[0].contaNum == 1234567890 and args[0].senha == 1234:
            func(*args)
        else:
            print("\033[1;31mVocê não tem permissão para acessar essa conta! Verifique suas credenciais.\033[1;m")
            
    return wrapper


class Banco():
    def __init__(self, instituicao, agenciaNum, contaNum, senha, nome, saldo):
        # Atributos
        self.instituicao = instituicao
        self.agenciaNum = agenciaNum
        self.contaNum = contaNum
        self.senha = senha
        self.nome = nome
        self.saldo = saldo
        self.extrato = []
    
    @loggin
    def consultarSaldo(self):
        print(f"\033[1;34mSaldo: {self.saldo}\033[1;m")
    
    @loggin
    def consultarInfos(self):
        print(f"\033[1;34mProprietario: {self.nome}\nInstituição Bancária: {self.instituicao}\nNúmero da Agência: {self.agenciaNum}\nNúmero da Conta: {self.contaNum}\033[1;m")
    
    @loggin
    @operacoes
    def deposito(self, quantia):
        self.saldo + quantia
        return {"Deposito": quantia}
    
    @loggin
    @operacoes
    def saque(self, quantia):
        self.saldo - quantia
        return {"Saque": quantia}
    
    @loggin
    def verExtrato(self):
        print("\033[1;33mAs últimas operações realizadas foram: \033[1;m")
        for op in self.extrato:
            print(f"\033[1;33m{op}\033[1;m")


if __name__ == "__main__":
    pessoa = Banco("Power Bank", 4321, 1234567890, 1234, "Paulo Vinicius Gomes da Silva", 200)

    pessoa.deposito(500)
    pessoa.saque(500)
    pessoa.deposito(1000)

    pessoa.consultarInfos()
    pessoa.consultarSaldo()
    
    pessoa.verExtrato()


