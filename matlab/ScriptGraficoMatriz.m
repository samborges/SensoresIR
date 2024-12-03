clc
clear all
close all

% Nome do arquivo CSV
filename = 'arrays\temperatura_AMG_8833\Array_DadosAluminioNatural_200mm_AMG8833.txt';

% Ler o arquivo CSV
data = readmatrix(filename, 'Delimiter', ';');
temperaturaColunaBruto = data(:,36);

% Aplicar o filtro de média móvel com 5 posições
% para cada ponto na série de dados, o script calcula a média dos 10 valores anteriores a esse ponto,
% incluindo o ponto atual. Isso resulta em um novo vetor filtroMediaMovel onde cada valor é a média
% dos 10 valores anteriores a ele na série original.
filtroMediaMovel = movmean(data,[5 0]);

% Extrair a coluna mais relevante da coleta
colunaRelevante = filtroMediaMovel(:, 36);
temperaturaMediaMovel = colunaRelevante(end,:);
disp('Dados media móvel Ponto mais relevante');
disp(temperaturaMediaMovel);


% Calcular a média dos dados da quarta coluna
media_coluna = filtroMediaMovel(end,:);
% disp('Dados media móvel matrix 8X8:');
% disp(media_coluna);

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
figure;
heatmap(matriz_8x8_Rotate);
title('Mapa de Calor Sensor AMG 8833');
colormap('cool')
xlabel('Colunas');
ylabel('Base');

% Criar o gráfico de linha comparando os dados originais e filtrados
figure;
hold on;
plot(temperaturaColunaBruto, 'b-.', 'Color', [0.0, 0.0, 1], 'LineWidth', 0.5,'DisplayName', 'Dados Originais');
plot(colunaRelevante, '-', 'Color', [0.00,1.00,1.00], 'LineWidth', 1, 'DisplayName', 'Média Móvel');
yline(temperaturaMediaMovel, '-', 'Color', [0.93, 0.69, 0.13], 'LineWidth', 2, 'DisplayName', 'Valor Médio', 'Alpha',0.5);

% Adicionar a linha de referência de temperatura camera Termica
yline(30.6, '-.', 'Color', [0.0, 0.7, 0.0], 'LineWidth', 0.5, 'DisplayName', 'Câmera Térmica');


% Adicionar a linha de referência de temperatura camera Termica
yline(44, '-.', 'Color', [0.93,0.69,0.13], 'LineWidth', 0.5, 'DisplayName', 'Termômetro');

% Adicionar o valor médio no gráfico
x_position = length(colunaRelevante)/2; % Posição X para o texto
y_position = media_coluna + 50 ; % Ajustar a posição Y acima da linha
% text(x_position, y_position, sprintf('Média: %.2f', media_coluna),'HorizontalAlignment', 'center', 'FontSize', 10, 'Color', [0.0, 0.0, 0.0]);
ylim([15 80]);
title('Comparação dos Dados Originais e Filtrados (Vidro Branco 200mm)');
xlabel('Tempo - seg');
ylabel('Valor - mm');
legend('show');
grid on;
hold off;

% Exibir o valor médio
disp('Valor médio da coluna:');
disp(media_coluna);
% Exibir o array formatado
fprintf('Array lido do arquivo: \n');
fprintf('%f ', colunaRelevante);
fprintf('\n');

