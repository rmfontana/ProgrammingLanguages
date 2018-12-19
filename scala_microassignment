//Reads userinput string and parses it with BNF grammar to build parse tree

package example

import scala.util.parsing.combinator._

abstract class Tree
case class Expression(l: Tree, r: Option[Tree]) extends Tree
case class Const(v: String) extends Tree

object Main extends Combinators{
  def main(args: Array[String]){

    val exp = scala.io.StdIn.readLine("expression?")

    try
    {
      val exp2:Tree = parseAll(e, exp).get
      println(exp2)
    } catch {
      case e: Exception => println("Expression doesn't match grammar, couldn't build parse tree")
    }
  }

}

class Combinators extends JavaTokenParsers{
  // E := C E | C
  // C := 'a' | 'b'
  def e: Parser[Tree] = c ~ e ^^ { case l ~ r => Expression(l, Option(r)) } | c
  def c: Parser[Tree] = const

  def const: Parser[Const] = "[a|b]".r ^^ { str => Const(str) }
}
