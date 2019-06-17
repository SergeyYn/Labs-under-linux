.PHONY: delete build rebuild run default

default:rebuild run
rebuild: delete build
delete:
	rm -f JAVA_PRO/*.class main.jar
build: main.jar
main.jar: JAVA_PRO/Main.class JAVA_PRO/In_out.class JAVA_PRO/Func.class JAVA_PRO/Array.class
	jar -cfm main.jar JAVA_PRO/manifest JAVA_PRO/Main.class JAVA_PRO/In_out.class JAVA_PRO/Func.class JAVA_PRO/Array.class
JAVA_PRO/Array.class: JAVA_PRO/Array.java
	javac JAVA_PRO/Array.java
JAVA_PRO/Main.class: JAVA_PRO/Main.java
	javac JAVA_PRO/Main.java
JAVA_PRO/In_out.class: JAVA_PRO/Array.class JAVA_PRO/In_out.java
	javac JAVA_PRO/In_out.java
JAVA_PRO/Func.class: JAVA_PRO/Array.class JAVA_PRO/Func.java
	javac JAVA_PRO/Func.java
run: main.jar
	java -jar main.jar

