
from collections import namedtuple
class LEX:
    def __init__(self, sentence_) -> None:
        self.sentence = sentence_
        self.next_token_type = 0
        self.ch = ""
        self.lexeme = ""
        self.char_class = 0

        self.CharType = namedtuple("CharType",['type','value'])
        self.digit = self.CharType("DIGIT", 1)
        self.unknown = self.CharType("UNKNOWN", 0)
        self.op = self.CharType("OP", 99)
        self.eof = self.CharType("EOF", -1)

        self.add_op = self.CharType("ADD_OP", 21)
        self.sub_op = self.CharType("SUB_OP", 22)
        self.mult_op = self.CharType("MULT_OP", 23)
        self.div_op = self.CharType("DIV_OP", 24)
        self.left_paren = self.CharType("LEFT_PAREN", 25)
        self.right_paren = self.CharType("RIGHT_PAREN", 26)

    def get_char(self):
        if len(self.sentence) > 0:
            self.ch = self.sentence.pop(0)
            if(self.ch.isdigit()): self.char_class = self.digit
            elif(self.ch.isalpha()): self.char_class = self.unknown
            else: self.char_class = self.op
        
        else:
            self.char_class = self.eof
    

    def lex(self):
        self.lexeme = ""
        if self.char_class == self.digit:
            while(self.char_class == self.digit):
                self.lexeme += self.ch
                self.get_char()

            self.next_token_type = self.digit

        elif self.char_class == self.op:
            OP_DICT = [self.add_op, self.sub_op, self.mult_op, self.div_op, self.left_paren, self.right_paren]
            for type_, char in zip(OP_DICT, "+-*/()"):
                if self.ch == char:
                    self.lexeme += char
                    self.next_token_type = type_
                    self.get_char()
                    break
        
        elif self.char_class == self.eof:
            self.next_token_type = self.eof
        
        else:
            raise Exception("Syntax Error!")
        
        print(f"Next token is : {self.next_token_type}, Next lexeme is : {self.lexeme}")

class Parser:
    def __init__(self, lex_) -> None:
        self.lex = lex_ #lex is class
    
    def number(self):
        self.result = 0
        return int(self.lex.lexeme)

    def expr(self):
        lex = self.lex
        result = self.term()
        while (lex.next_token_type == lex.add_op or lex.next_token_type == lex.sub_op):
            if lex.next_token_type == lex.add_op:
                lex.lex()
                result += self.factor()
            if lex.next_token_type == lex.sub_op:
                lex.lex()
                result -= self.factor()
        return result

    def factor(self):
        flag = 1
        lex = self.lex
        if lex.next_token_type == lex.sub_op:
            lex.lex()
            flag = -1
        
        if lex.next_token_type == lex.left_paren:
            lex.lex()
            result = self.expr()
            if(lex.next_token_type == lex.right_paren):
                lex.lex()
            else: 
                raise Exception("Syntax Error!")

        elif (lex.next_token_type == lex.digit):
            result = self.number()
            lex.lex()

        else: raise Exception("Syntax Error!")
        
        return result * flag

    def term(self):
        result = self.factor()
        lex = self.lex
        while (lex.next_token_type == lex.mult_op or lex.next_token_type == lex.div_op):
            if lex.next_token_type == lex.mult_op:
                lex.lex()
                result *= self.factor()
            if lex.next_token_type == lex.div_op:
                lex.lex()
                result /= self.factor()
        return result
    
if __name__ == '__main__':
    s = [i for i in input() if i != ' ']
    lex = LEX(s)
    lex.get_char()
    lex.lex()
    parser = Parser(lex)
    while lex.next_token_type != lex.eof:
        print("result is : ", parser.expr())