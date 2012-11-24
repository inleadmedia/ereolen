<?php
/**
 * @file
 *
 */
?>
<ul class="publizon-loans-list">
  <?php foreach ($loans as $loan): ?>
  <li>
    <?php echo theme('publizon_loan', $loan); ?>
  </li>
  <?php endforeach; ?>
</ul>