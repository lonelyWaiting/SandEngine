#pragma once

void shader_init(struct ID3D11Device* device, const char* shader_parser_search_path = nullptr);
void shader_cleanup();
bool shader_load(const char* filename);
void shader_add_search_path(const char* path);
