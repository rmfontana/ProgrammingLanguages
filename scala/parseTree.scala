// Simplifies logical expressions from user input by parsing through tree through the use of case classes.
// Accepts and, or and not (&&, ||, !)

package example

import java.text.ParseException

import scala.util.parsing.combinator._

abstract class Tree
case class Or(l: Tree, r: Tree) extends Tree
case class And(l: Tree, r: Tree) extends Tree
case class Not(l: Tree, r: Option[Tree]) extends Tree
case class Par(l: Tree, r: Option[Tree]) extends Tree

case class Const(n: String) extends Tree
case class Bool(n: Boolean) extends Tree

object Main extends Combinators{

  def oar(l:String, r:String): String = {
    if(l == "true" || l == "(true)"){
      "true"// true or anything
    } else if (l == "false" || l == "(false)"){
      if(r == "false" || r == "(false)"){
        "false"// false or false
      } else if(r == "true" || r == "(true)"){
        "true"// false or true
      } else {
        r // false or symbol
      }
    } else {
      if (r == "false" || r == "(false)") {
        l // symbol or false
      } else if (r == "true" || r == "(true)") {
        "true" // symbol or true
      } else {
        if (l.length == 3 && l.takeRight(1) == ")") {
          l.substring(1, l.length - 1) + " || " + r
        } else if (r.length == 3 && r.takeRight(1) == ")") {
          l + " || " + r.substring(1, r.length - 1)
        } else {
          l + " || " + r // symbol and symbol
        }
      }
    }
  }

  def nd(l: String, r: String): String = {
    if(l == "true" || l == "(true)"){
      if(r == "true" || r == "(true)"){
        "true" // true and true
      } else if(r == "false" || r == "(false)") {
        "false" // true and false
      } else {
        r // true and symbol
      }
    } else if(l == "false" || l == "(false)"){
      "false" // false and anything
    } else {
      if(r == "false" || r == "(false)"){
        "false" // symbol and false
      } else if(r == "true" || r == "(true)"){
        l // symbol and true
      } else {
        if(l.length == 3 && l.takeRight(1) == ")"){
          l.substring(1, l.length - 1) + " && " + r
        } else if(r.length == 3 && r.takeRight(1) == ")") {
          l + " && " + r.substring(1, r.length - 1)
        } else {
          l + " && " + r // symbol and symbol
        }
      }
    }
  }

  def knot(l: String) : String = {
    if(l == "true"){
      "false"
    } else if(l == "false"){
      "true"
    } else {
      "!" + l
    }
  }


  def eval(t: Tree): String = t match {
    case Or(l, r) => oar(eval(l), eval(r))
    case And(l, r) => nd(eval(l), eval(r))
    case Not(l, None) => knot(eval(l))
    case Par(l, None) => eval(l)
    case Const(n) => n
    case Bool(n) => n.toString
  }

  def main(args: Array[String]) = {
    while (true) {
      val exp = scala.io.StdIn.readLine("expression?")

      try{
        val exp2c: Tree = parseAll(e, exp).get
        print(exp2c)
        print("\n")
        print(eval(exp2c))
        print("\n")
      } catch {
        case e: RuntimeException => print("No result when parsing failed. Expression doesn't match grammar.\n")
      }
    }
  }
}

class Combinators extends RegexParsers{
  /*
  E := T '||' E | T
  T := F '&&' T | F
  F := '!' A | A
  A := '(' E ')' | C
  C := 'true' | 'false' | c
  c := any alphabetic character
   */

  def e: Parser[Tree] = t ~ or ~ e ^^ { case l ~ or ~ r => Or(l, r)} | t
  def t: Parser[Tree] = f ~ and ~ t ^^ { case l ~ and ~ r => And(l, r)} | f
  def f: Parser[Tree] = nott ~ a ^^ { case not ~ l => Not(l, None)} | a
  def a: Parser[Tree] = spar ~ e ~ epar ^^ { case spar ~ e ~ epar => Par(e, None)} | C
  def C: Parser[Tree] = bool | c
  def c: Parser[Tree] = const

  def or[Tree] = "||"
  def and[Tree] = "&&"
  def nott[Tree] = "!"
  def spar[Tree] = "("
  def epar[Tree] = ")"


  def bool: Parser[Bool] =  "false".r ^^ { bool => Bool(false)} | "true".r ^^ { bool => Bool(true)}
  def const: Parser[Const] = "[A-Za-z]".r ^^ { str => Const(str)}
}
