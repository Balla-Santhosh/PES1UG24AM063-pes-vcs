int index_load(Index *index) {
    index->count = 0;

    FILE *f = fopen(INDEX_FILE, "r");
    if (!f) {
        // File doesn't exist yet — empty index, not an error
        return 0;
    }

    char hex[HASH_HEX_SIZE + 1];
    IndexEntry *e;

    while (index->count < MAX_INDEX_ENTRIES) {
        e = &index->entries[index->count];

        int ret = fscanf(f, "%o %64s %llu %u %511s",
                         &e->mode,
                         hex,
                         (unsigned long long *)&e->mtime_sec,
                         &e->size,
                         e->path);

        if (ret == EOF) break;
        if (ret != 5) { fclose(f); return -1; }

        if (hex_to_hash(hex, &e->hash) != 0) { fclose(f); return -1; }

        index->count++;
    }

    fclose(f);
    return 0;
}
