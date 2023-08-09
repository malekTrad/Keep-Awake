<!-- ... -->
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css" rel="stylesheet">

<table id="productTable" class="display">
    <!-- ... -->
    <tbody>
        % for product in products:
        <tr>
            <!-- ... -->
            <td id="description_${product[b'id'].decode()}">${product[b'description'].decode()}</td>
            <td>
                % if not b'deleted' in product:
                    <i class="fas fa-edit editIcon" data-id="${product[b'id'].decode()}"></i>
                % endif
            </td>
        </tr>
        % endfor
    </tbody>
</table>
<!-- ... -->
<script>
    $(document).ready(function() {
        $('#productTable').DataTable();

        // Handle edit icon clicks
        $('.editIcon').on('click', function() {
            var productId = $(this).data('id');
            openEditDialog(productId);
        });

        function openEditDialog(productId) {
            var description = $('#description_' + productId).text();
            var editInput = '<input type="datetime-local" id="editInput_' + productId + '" value="' + description + '">';
            var saveIcon = '<i class="fas fa-save saveIcon" data-id="' + productId + '"></i>';
            $('#description_' + productId).html(editInput + saveIcon);

            $('.saveIcon').on('click', function() {
                var productId = $(this).data('id');
                var newDescription = $('#editInput_' + productId).val();
                updateProductDescription(productId, newDescription);
            });
        }

        function updateProductDescription(productId, newDescription) {
            var formData = new FormData();
            formData.append('id', productId);
            formData.append('description', newDescription);

            fetch('/update', {
                method: 'POST',
                body: formData
            })
            .then(response => response.text())
            .then(data => {
                if (data === 'ok') {
                    alert('Product description updated successfully.');
                    // You may want to refresh the table or update the UI here
                } else {
                    alert('Error updating product description.');
                }
            })
            .catch(error => {
                console.error('Error updating product description:', error);
            });
        }
    });
</script>
<!-- ... -->
