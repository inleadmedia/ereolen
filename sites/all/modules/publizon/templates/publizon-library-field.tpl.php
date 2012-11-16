<?php
/**
 * @file
 * Defualt template file for the publizon library field.
 */
?>
<div id="<?php echo $element['#id'] . '-wrapper'; ?>" class="form-item form-item-labeled publizon-library-field">
  <div class="retailer-id">
    <label for="<?php echo $element['#id'] . '-retailer-id'; ?>">
      <?php echo t('Retailer ID'); ?>
      <span class="form-required" title="This field is required.">*</span>
    </label>
    <input id="<?php echo $element['#id'] . '-retailer-id'; ?>" class="form-text required fuild" type="text" value="<?php echo $element['#value']['retailer_id']; ?>" size="6" name="<?php echo $element['#name'] . '[retailer_id]' ?>" maxlenght="128" />
  </div>

  <div class="retailer-key-code">
    <label for="<?php echo $element['#id'] . '-retailer-key-code'; ?>">
      <?php echo t('Retailer key code'); ?>
      <span class="form-required" title="This field is required.">*</span>
    </label>
    <input id="<?php echo $element['#id'] . '-retailer-key-code'; ?>" class="form-text required fuild" type="text" value="<?php echo $element['#value']['retailer_key_code']; ?>" size="14" name="<?php echo $element['#name'] . '[retailer_key_code]' ?>" maxlenght="128" />
  </div>

  <div class="library-name">
    <label for="<?php echo $element['#id'] . '-library-name'; ?>">
      <?php echo t('Library name'); ?>
      <span class="form-required" title="This field is required.">*</span>
    </label>
    <input id="<?php echo $element['#id'] . '-library-name'; ?>" class="form-text required fuild" type="text" value="<?php echo $element['#value']['library_name']; ?>" size="20" name="<?php echo $element['#name'] . '[library_name]' ?>" maxlenght="128" />
  </div>

  <div class="max-loans">
    <label for="<?php echo $element['#id'] . '-max-loans'; ?>">
      <?php echo $element['#title'] ?>
      <span class="form-required" title="This field is required.">*</span>
    </label>
    <input id="<?php echo $element['#id'] . '-max-loans'; ?>" class="form-text required fuild" type="text" value="<?php echo $element['#value']['max_loans']; ?>" size="12" name="<?php echo $element['#name'] . '[max_loans]' ?>" maxlenght="128" />
  </div>
  <div class="description"><?php echo $element['#description']; ?></div>
</div>