clc
clear all
close all

% Nome do arquivo CSV
filename = 'arrays\distancia_VL53L5cx\Array_PlasticoLiquidoPreto_200mm_VL53L5.txt';

numeroColuna = 20;

% Ler o arquivo CSV
data = readmatrix(filename, 'Delimiter', ';');
distanciaColunaBruto = data(:,numeroColuna);

% Aplicar o filtro de média móvel com 5 posições
% para cada ponto na série de dados, o script calcula a média dos 10 valores anteriores a esse ponto,
% incluindo o ponto atual. Isso resulta em um novo vetor filtroMediaMovel onde cada valor é a média
% dos 10 valores anteriores a ele na série original.
filtroMediaMovel = movmean(data,[5 0]);

% Extrair a coluna mais relevante da coleta
colunaRelevante = filtroMediaMovel(:, numeroColuna);
distanciaMediaMovel = colunaRelevante(end,:);
disp('Dados media móvel Ponto mais relevante');
disp(distanciaMediaMovel);

% Calcular a média dos dados da colunas
media_coluna = filtroMediaMovel(end,:);
disp('Dados media móvel Ponto mais relevante');
disp(media_coluna);


% Verificar se temos pelo menos 64 dados para preencher a matriz 8x8
if numel(media_coluna) < 64
    error('Não há dados suficientes para preencher a matriz 8x8.');
end

% Selecionar os primeiros 64 dados da média móvel
dados_para_heatmap = media_coluna(1:64);

% Reformatar os dados em uma matriz 8x8
matriz_8x8 = reshape(dados_para_heatmap, [8, 8]);
matriz_8x8_Rotate = rot90(matriz_8x8, 1);

% Criar o mapa de calor
f1 = figure;
f1.Position(3:4) = [600 420];
heatmap(matriz_8x8_Rotate);
title('Sensor VL53L5 (Plástico Preto 200mm) - Mapa distância (mm)');
colormap(flip(cool));

xlabel('Base');
ylabel('Laterais');

% Criar o gráfico de linha comparando os dados originais e filtrados
f2 = figure;
hold on;
f2.Position(3:4) = [800 420];

plot(distanciaColunaBruto, 'b-.', 'Color', [0.0, 0.0, 1], 'LineWidth', 0.5,'DisplayName', 'Dados Originais');
plot(colunaRelevante, '-', 'Color', [0.00,1.00,1.00], 'LineWidth', 1, 'DisplayName', 'Dados Filtrados');
yline(distanciaMediaMovel, '-', 'Color', [0, 0, 0], 'LineWidth', 1.5, 'DisplayName', 'Valor Médio', 'Alpha',0.5);

distancia = 200;

% Adicionar o valor médio no gráfico
x_position = length(colunaRelevante)/3; % Posição X para o texto
y_position = distanciaMediaMovel + 30 ; % Ajustar a posição Y acima da linha
text(x_position, y_position, sprintf('Distância Média Móvel: %.2f', distanciaMediaMovel),'HorizontalAlignment', 'center', 'FontSize', 14, 'Color', [0.0, 0.0, 0.0]);
% Adicionar a linha de referência de distancia
yline(distancia, ':', 'Color', [1, 0, 0], 'LineWidth', 2, 'DisplayName', 'Distância Régua');
x_position = length(colunaRelevante)/2; % Posição X para o texto
y_position = distancia - 5 ; % Ajustar a posição Y acima da linha
text(x_position, y_position, sprintf('Distância na Régua do Sensor: %.2f', distancia),'HorizontalAlignment', 'center', 'FontSize', 14, 'Color', [0.0, 0.0, 0.0]);

ylim([180 280]);
title('Sensor VL53L5 - Comparação dos Dados Originais e Filtrados (Plástico Preto 200mm)');
xlabel('Períodos Medidos (Und)');
ylabel('Valor mm');
legend('show','FontSize',12);

grid on;
hold off;