CREATE DATABASE Educacao;

USE Educacao;
CREATE TABLE Endereco (
	id INT AUTO_INCREMENT PRIMARY KEY,
    logra VARCHAR(80),
    cidade VARCHAR(80),
    numero INT,
    cep INT
);

CREATE TABLE Professor (
	id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(80),
    mat INT,
    freq INT,
    salario INT,
	id_endereco INT,
    FOREIGN KEY (id_endereco) REFERENCES Endereco(id)
);

CREATE TABLE Aluno (
	id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(80),
    mat INT,
    freq INT,
    nota FLOAT,
    id_endereco INT,
    FOREIGN KEY (id_endereco) REFERENCES Endereco(id)
);

CREATE TABLE Colegio (
	id INT AUTO_INCREMENT PRIMARY KEY,
    setor VARCHAR(80),
    id_endereco INT,
    id_aluno INT,
    id_professor INT,
    FOREIGN KEY (id_endereco) REFERENCES Endereco(id),
    FOREIGN KEY (id_aluno) REFERENCES Aluno(id),
    FOREIGN KEY (id_professor) REFERENCES Professor(id)
);


# ---------- Divisor de Águas ----------

SELECT * FROM endereco;
SELECT * FROM aluno where id = 2;
SELECT * FROM professor;
SELECT * FROM colegio;

update endereco set logra = 'Rua Antônio Falcão' where id = 3;

# ---------- Divisor de Águas ----------

INSERT INTO Endereco (logra, cidade, numero, cep) VALUES
('Rua das Flores', 'João Pessoa', 123, 58000000),
('Av. Brasil', 'Recife', 456, 50000000),
('Rua A', 'Natal', 789, 59000000),
('Rua B', 'Fortaleza', 321, 60000000);

# -- Professores
INSERT INTO Professor (nome, mat, freq, salario, id_endereco) VALUES
('Carlos Silva', 1001, 40, 1100, 1),
('Ana Souza', 1002, 38, 1080, 2),
('Carla Sá', 1003, 40, 5000, 1);

# -- Alunos
INSERT INTO Aluno (nome, mat, freq, nota, id_endereco) VALUES
('João Pedro', 2001, 35, 8.5, 3),
('Maria Clara', 2002, 36, 9.2, 4),
('João Sá', 2003, 31, 7.2, 1);

# -- Colegio
INSERT INTO Colegio (setor, id_endereco, id_aluno, id_professor) VALUES
('Ensino Fundamental', 1, 1, 1),
('Ensino Médio', 2, 2, 2),
('Graduação', 1, 3, 2);

START TRANSACTION;
-- OPERAÇÃO UPDATE, DELETE OR INSERT
INSERT INTO Aluno (nome, mat, freq, nota, id_endereco) VALUES
('Laura Diniz', 2029, 35, 9.5, 1)

COMMIT; -- SE OCORRER TUDO CERTO COMMIT
ROLLBACK; -- CASO CONTRÁRIO, ROLLBACK

-- FUNÇÕES AGREGADORAS

SELECT COUNT(*)
FROM Endereco;

SELECT MAX(salario)
FROM Professor;

SELECT id, MAX(salario) as salario_mes
FROM Professor;

SELECT MIN(salario)
FROM Professor;

SELECT SUM(salario) as soldo
FROM Professor;

SELECT AVG(salario) as media_salario
FROM Professor;

SELECT ROUND(AVG(salario)) as media_salario
FROM Professor;
