
clc
clear all
close all

% Ler o arquivo TXT
filename = 'arrays\temperatura_AMG_8833\Array_DadosAluminioNatural_200mm_AMG8833.txt';

% Ler o arquivo TXT
data = readmatrix(filename, 'Delimiter', ';');

% Exibir os dados lidos
disp('Dados lidos do arquivo:');
disp(data);

% Inicializar a array mediaMovelInicial com 64 índices
filtro_media_movel = zeros(1, 64);

% Preencher a array com os valores do arquivo CSV lido
for i = 1:min(64, numel(data))
    filtro_media_movel(i) = movmean(data(i), 10);
end


% Exibir a array mediaMovelInicial
disp('Array mediaMovelInicial:');
disp(filtro_media_movel);

%-------------------------------------------------------------------------%

% Verificar se temos pelo menos 64 dados para preencher a matriz 8x8
if numel(filtro_media_movel) < 64
    error('Não há dados suficientes para preencher a matriz 8x8.');
end

% Selecionar os primeiros 64 dados da média móvel
dados_para_heatmap = filtro_media_movel(1:64);

% Reformatar os dados em uma matriz 8x8
matriz_8x8 = reshape(dados_para_heatmap, [8, 8]);

% Criar o mapa de calor
figure;
heatmap(matriz_8x8);
title('Mapa de Calor da Média Móvel');
xlabel('Colunas');
ylabel('Linhas');
