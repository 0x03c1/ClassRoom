CREATE DATABASE Educacao;

USE Educacao;

CREATE TABLE Endereco (
	id int auto_increment primary key,
    logra varchar(80),
    cidade varchar(80),
    numero int,
    cep int
);

CREATE TABLE Professor (
	id int auto_increment primary key,
    nome varchar(80),
    mat int,
    freq int,
    id_endereco int,
    foreign key (id_endereco) references endereco(id)
);
    
CREATE TABLE Aluno (
	id int auto_increment primary key,
    nome varchar(80),
    mat int,
    freq int,
    nota float,
    id_endereco int,
    foreign key (id_endereco) references endereco(id)
);
    
CREATE TABLE Colegio (
	id int auto_increment primary key,
    setor varchar(80),
    id_endereco int,
    id_aluno int,
    id_professor int,
foreign key (id_endereco) references endereco(id),
foreign key (id_aluno) references aluno(id),
foreign key (id_professor) references professor(id));

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
INSERT INTO Professor (nome, mat, freq, id_endereco) VALUES
('Carlos Silva', 1001, 40, 1),
('Ana Souza', 1002, 38, 2),
('Carla Sá', 1003, 40, 1);

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
